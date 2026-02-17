#include "sipaccount.h"
#include "../utils/logger.h"

SipAccount::SipAccount(QObject *parent)
    : QObject(parent)
    , pj::Account()
    , m_registered(false)
{
}

SipAccount::~SipAccount()
{
}

void SipAccount::onRegState(pj::OnRegStateParam &prm)
{
    pj::AccountInfo ai = getInfo();
    m_registered = (ai.regStatus == PJSIP_SC_OK);

    QString reason = QString::fromStdString(ai.regStatusText);
    Logger::instance().log(Logger::Info,
        QString("Account %1: %2 (%3)")
            .arg(QString::fromStdString(ai.uri))
            .arg(m_registered ? "registered" : "unregistered")
            .arg(ai.regStatus));

    emit registrationStateChanged(m_registered, ai.regStatus, reason);
}

void SipAccount::onIncomingCall(pj::OnIncomingCallParam &iprm)
{
    Logger::instance().log(Logger::Info,
        QString("Incoming call: %1").arg(iprm.callId));
    emit incomingCall(iprm.callId);
}
