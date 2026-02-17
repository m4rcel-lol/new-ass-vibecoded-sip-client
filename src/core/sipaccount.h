#ifndef SIPACCOUNT_H
#define SIPACCOUNT_H

#include <pjsua2.hpp>

// PJSIP headers may define 'emit', 'signals', 'slots' macros that conflict with Qt
// Only undef if QObject hasn't been included yet (to avoid removing Qt's definitions)
#ifndef QOBJECT_H
#ifdef signals
#undef signals
#endif
#ifdef slots
#undef slots
#endif
#ifdef emit
#undef emit
#endif
#endif

#include <QObject>

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
