#ifndef SIPACCOUNT_H
#define SIPACCOUNT_H

#include <QObject>
#include <pjsua2.hpp>

/**
 * @brief The SipAccount class represents a SIP account
 */
class SipAccount : public QObject, public pj::Account
{
    Q_OBJECT

public:
    explicit SipAccount(QObject *parent = nullptr);
    ~SipAccount() override;

    // pj::Account callbacks
    void onRegState(pj::OnRegStateParam &prm) override;
    void onIncomingCall(pj::OnIncomingCallParam &iprm) override;

signals:
    void registrationStateChanged(bool registered, int code, const QString &reason);
    void incomingCall(int callId);

private:
    bool m_registered;
};

#endif // SIPACCOUNT_H
