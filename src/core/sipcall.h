#ifndef SIPCALL_H
#define SIPCALL_H

#include <QObject>
#include <pjsua2.hpp>

/**
 * @brief The SipCall class represents an active SIP call
 */
class SipCall : public QObject, public pj::Call
{
    Q_OBJECT

public:
    explicit SipCall(pj::Account &account, int call_id = PJSUA_INVALID_ID);
    ~SipCall() override;

    // pj::Call callbacks
    void onCallState(pj::OnCallStateParam &prm) override;
    void onCallMediaState(pj::OnCallMediaStateParam &prm) override;
    void onDtmfDigit(pj::OnDtmfDigitParam &prm) override;

    // Call control
    void hold();
    void unhold();
    void mute(bool muted);

signals:
    void stateChanged(int state);
    void mediaStateChanged(bool hasAudio, bool hasVideo);
    void dtmfReceived(const QString &digit);

private:
    bool m_onHold;
    bool m_muted;
};

#endif // SIPCALL_H
