#ifndef SIPENGINE_H
#define SIPENGINE_H

#include <pjsua2.hpp>

// PJSIP headers define 'emit', 'signals', 'slots' macros that conflict with Qt
// Undefine them here before including Qt headers, Qt will redefine them properly
#ifdef signals
#undef signals
#endif
#ifdef slots
#undef slots
#endif
#ifdef emit
#undef emit
#endif

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QList>
#include "sipaccount.h"
#include "sipcall.h"

/**
 * @brief The SipEngine class manages the PJSIP library and all SIP operations
 *
 * This class runs in a separate thread from the UI to ensure non-blocking operation.
 * It follows the singleton pattern and provides thread-safe access to SIP functionality.
 */
class SipEngine : public QObject
{
    Q_OBJECT

public:
    static SipEngine& instance();

    // Lifecycle
    bool initialize();
    void shutdown();

    // Account management
    bool addAccount(const QString &sipUri, const QString &username, const QString &password,
                    const QString &server, int port = 5060, bool useTls = false);
    bool removeAccount(const QString &accountId);
    QList<SipAccount*> accounts() const;

    // Call operations
    bool makeCall(const QString &accountId, const QString &destination);
    bool answerCall(const QString &callId);
    bool hangupCall(const QString &callId);
    bool holdCall(const QString &callId, bool hold);
    bool muteCall(const QString &callId, bool mute);
    bool transferCall(const QString &callId, const QString &destination);
    bool attendedTransfer(const QString &callId, const QString &targetCallId);
    bool sendDtmf(const QString &callId, const QString &digits);

    // Status
    bool isInitialized() const { return m_initialized; }
    QList<SipCall*> activeCalls() const;

signals:
    void initialized();
    void shutdownComplete();
    void accountAdded(const QString &accountId);
    void accountRemoved(const QString &accountId);
    void accountRegistrationState(const QString &accountId, bool registered);
    void incomingCall(const QString &callId, const QString &from);
    void callStateChanged(const QString &callId, int state);
    void callMediaState(const QString &callId, bool hasAudio, bool hasVideo);
    void error(const QString &message);

private:
    SipEngine();
    ~SipEngine() override;
    Q_DISABLE_COPY(SipEngine)

    void initializePjsip();
    void cleanupPjsip();

    bool m_initialized;
    QThread *m_workerThread;
    QMutex m_mutex;

    pj::Endpoint m_endpoint;
    QList<SipAccount*> m_accounts;
    QList<SipCall*> m_calls;
};

#endif // SIPENGINE_H
