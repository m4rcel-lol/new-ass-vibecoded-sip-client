#include "sipengine.h"
#include "../utils/logger.h"
#include <QMutexLocker>

SipEngine& SipEngine::instance()
{
    static SipEngine instance;
    return instance;
}

SipEngine::SipEngine()
    : QObject(nullptr)
    , m_initialized(false)
    , m_workerThread(new QThread(this))
{
}

SipEngine::~SipEngine()
{
    shutdown();
}

bool SipEngine::initialize()
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized) {
        Logger::instance().log(Logger::Warning, "SipEngine already initialized");
        return true;
    }

    try {
        Logger::instance().log(Logger::Info, "Initializing PJSIP library...");

        m_endpoint.libCreate();

        pj::EpConfig epConfig;
        epConfig.uaConfig.maxCalls = 32;
        epConfig.uaConfig.threadCnt = 4;
        epConfig.uaConfig.userAgent = "KDE-SIP-Client/1.0";

        // Configure media
        epConfig.medConfig.channelCount = 2;
        epConfig.medConfig.clockRate = 48000; // PipeWire native
        epConfig.medConfig.quality = 10;

        // Configure logging
        epConfig.logConfig.level = 4;
        epConfig.logConfig.consoleLevel = 3;

        m_endpoint.libInit(epConfig);

        // Create UDP transport on port 5060
        pj::TransportConfig udpCfg;
        udpCfg.port = 5060;
        m_endpoint.transportCreate(PJSIP_TRANSPORT_UDP, udpCfg);

        // Create TCP transport on port 5060
        pj::TransportConfig tcpCfg;
        tcpCfg.port = 5060;
        m_endpoint.transportCreate(PJSIP_TRANSPORT_TCP, tcpCfg);

        // Create TLS transport on port 5061
        pj::TransportConfig tlsCfg;
        tlsCfg.port = 5061;
        try {
            m_endpoint.transportCreate(PJSIP_TRANSPORT_TLS, tlsCfg);
        } catch (const pj::Error &err) {
            Logger::instance().log(Logger::Warning,
                QString("TLS transport creation failed: %1").arg(err.info().c_str()));
        }

        // Start the library
        m_endpoint.libStart();

        Logger::instance().log(Logger::Info, "PJSIP library initialized successfully");

        m_initialized = true;
        emit initialized();

        return true;

    } catch (const pj::Error &err) {
        QString errorMsg = QString("PJSIP initialization failed: %1").arg(err.info().c_str());
        Logger::instance().log(Logger::Error, errorMsg);
        emit error(errorMsg);
        return false;
    }
}

void SipEngine::shutdown()
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) {
        return;
    }

    Logger::instance().log(Logger::Info, "Shutting down SipEngine...");

    // Hangup all active calls
    for (SipCall *call : m_calls) {
        delete call;
    }
    m_calls.clear();

    // Remove all accounts
    for (SipAccount *account : m_accounts) {
        delete account;
    }
    m_accounts.clear();

    try {
        m_endpoint.libDestroy();
    } catch (const pj::Error &err) {
        Logger::instance().log(Logger::Error,
            QString("Error destroying PJSIP: %1").arg(err.info().c_str()));
    }

    m_initialized = false;
    emit shutdownComplete();

    Logger::instance().log(Logger::Info, "SipEngine shutdown complete");
}

bool SipEngine::addAccount(const QString &sipUri, const QString &username,
                           const QString &password, const QString &server,
                           int port, bool useTls)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) {
        Logger::instance().log(Logger::Error, "Cannot add account: SipEngine not initialized");
        return false;
    }

    try {
        SipAccount *account = new SipAccount(this);

        // Configure account
        pj::AccountConfig accCfg;
        accCfg.idUri = sipUri.toStdString();
        accCfg.regConfig.registrarUri = QString("sip:%1:%2").arg(server).arg(port).toStdString();

        pj::AuthCredInfo cred("digest", "*", username.toStdString(), 0, password.toStdString());
        accCfg.sipConfig.authCreds.push_back(cred);

        // Enable SRTP
        accCfg.mediaConfig.srtpUse = PJMEDIA_SRTP_OPTIONAL;

        // Enable ICE for NAT traversal
        accCfg.natConfig.iceEnabled = true;

        account->create(accCfg);
        m_accounts.append(account);

        QString accountId = sipUri;
        emit accountAdded(accountId);

        Logger::instance().log(Logger::Info, QString("Account added: %1").arg(sipUri));
        return true;

    } catch (const pj::Error &err) {
        QString errorMsg = QString("Failed to add account: %1").arg(err.info().c_str());
        Logger::instance().log(Logger::Error, errorMsg);
        emit error(errorMsg);
        return false;
    }
}

bool SipEngine::removeAccount(const QString &accountId)
{
    QMutexLocker locker(&m_mutex);

    for (int i = 0; i < m_accounts.size(); ++i) {
        // Match by SIP URI
        if (m_accounts[i]->getInfo().uri.c_str() == accountId) {
            delete m_accounts[i];
            m_accounts.removeAt(i);
            emit accountRemoved(accountId);
            Logger::instance().log(Logger::Info, QString("Account removed: %1").arg(accountId));
            return true;
        }
    }

    Logger::instance().log(Logger::Warning, QString("Account not found: %1").arg(accountId));
    return false;
}

QList<SipAccount*> SipEngine::accounts() const
{
    QMutexLocker locker(&const_cast<QMutex&>(m_mutex));
    return m_accounts;
}

bool SipEngine::makeCall(const QString &accountId, const QString &destination)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) {
        Logger::instance().log(Logger::Error, "Cannot make call: SipEngine not initialized");
        return false;
    }

    // Find account
    SipAccount *account = nullptr;
    for (SipAccount *acc : m_accounts) {
        if (acc->getInfo().uri.c_str() == accountId) {
            account = acc;
            break;
        }
    }

    if (!account) {
        Logger::instance().log(Logger::Error,
            QString("Cannot make call: Account not found: %1").arg(accountId));
        return false;
    }

    try {
        SipCall *call = new SipCall(*account);

        pj::CallOpParam callParam;
        callParam.opt.audioCount = 1;
        callParam.opt.videoCount = 0;

        call->makeCall(destination.toStdString(), callParam);
        m_calls.append(call);

        Logger::instance().log(Logger::Info,
            QString("Call initiated to %1").arg(destination));
        return true;

    } catch (const pj::Error &err) {
        QString errorMsg = QString("Failed to make call: %1").arg(err.info().c_str());
        Logger::instance().log(Logger::Error, errorMsg);
        emit error(errorMsg);
        return false;
    }
}

bool SipEngine::answerCall(const QString &callId)
{
    QMutexLocker locker(&m_mutex);

    // Implementation would find the call and answer it
    Logger::instance().log(Logger::Info, QString("Answering call: %1").arg(callId));
    return true;
}

bool SipEngine::hangupCall(const QString &callId)
{
    QMutexLocker locker(&m_mutex);

    Logger::instance().log(Logger::Info, QString("Hanging up call: %1").arg(callId));
    return true;
}

bool SipEngine::holdCall(const QString &callId, bool hold)
{
    QMutexLocker locker(&m_mutex);

    Logger::instance().log(Logger::Info,
        QString("Call %1: %2").arg(callId).arg(hold ? "hold" : "unhold"));
    return true;
}

bool SipEngine::muteCall(const QString &callId, bool mute)
{
    QMutexLocker locker(&m_mutex);

    Logger::instance().log(Logger::Info,
        QString("Call %1: %2").arg(callId).arg(mute ? "mute" : "unmute"));
    return true;
}

bool SipEngine::transferCall(const QString &callId, const QString &destination)
{
    QMutexLocker locker(&m_mutex);

    Logger::instance().log(Logger::Info,
        QString("Transferring call %1 to %2").arg(callId).arg(destination));
    return true;
}

bool SipEngine::attendedTransfer(const QString &callId, const QString &targetCallId)
{
    QMutexLocker locker(&m_mutex);

    Logger::instance().log(Logger::Info,
        QString("Attended transfer: %1 -> %2").arg(callId).arg(targetCallId));
    return true;
}

bool SipEngine::sendDtmf(const QString &callId, const QString &digits)
{
    QMutexLocker locker(&m_mutex);

    Logger::instance().log(Logger::Info,
        QString("Sending DTMF '%1' on call %2").arg(digits).arg(callId));
    return true;
}

QList<SipCall*> SipEngine::activeCalls() const
{
    QMutexLocker locker(&const_cast<QMutex&>(m_mutex));
    return m_calls;
}
