#include "sipcall.h"
#include "../utils/logger.h"

SipCall::SipCall(pj::Account &account, int call_id)
    : QObject(nullptr)
    , pj::Call(account, call_id)
    , m_onHold(false)
    , m_muted(false)
{
}

SipCall::~SipCall()
{
}

void SipCall::onCallState(pj::OnCallStateParam &prm)
{
    pj::CallInfo ci = getInfo();

    Logger::instance().log(Logger::Info,
        QString("Call %1 state: %2")
            .arg(ci.id)
            .arg(QString::fromStdString(ci.stateText)));

    emit stateChanged(ci.state);

    // Auto-cleanup on disconnected
    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        deleteLater();
    }
}

void SipCall::onCallMediaState(pj::OnCallMediaStateParam &prm)
{
    pj::CallInfo ci = getInfo();

    bool hasAudio = false;
    bool hasVideo = false;

    for (unsigned i = 0; i < ci.media.size(); ++i) {
        if (ci.media[i].type == PJMEDIA_TYPE_AUDIO) {
            hasAudio = true;

            if (ci.media[i].status == PJSUA_CALL_MEDIA_ACTIVE ||
                ci.media[i].status == PJSUA_CALL_MEDIA_REMOTE_HOLD) {

                pj::AudioMedia *aud_med = static_cast<pj::AudioMedia*>(getMedia(i));
                if (aud_med) {
                    try {
                        pj::AudDevManager &mgr = pj::Endpoint::instance().audDevManager();
                        aud_med->startTransmit(mgr.getPlaybackDevMedia());
                        mgr.getCaptureDevMedia().startTransmit(*aud_med);
                    } catch (const pj::Error &err) {
                        Logger::instance().log(Logger::Error,
                            QString("Audio media error: %1").arg(err.info().c_str()));
                    }
                }
            }
        } else if (ci.media[i].type == PJMEDIA_TYPE_VIDEO) {
            hasVideo = true;
        }
    }

    emit mediaStateChanged(hasAudio, hasVideo);
}

void SipCall::onDtmfDigit(pj::OnDtmfDigitParam &prm)
{
    QString digit = QString::fromStdString(prm.digit);
    Logger::instance().log(Logger::Info,
        QString("DTMF received: %1").arg(digit));
    emit dtmfReceived(digit);
}

void SipCall::hold()
{
    try {
        pj::CallOpParam prm;
        setHold(prm);
        m_onHold = true;
        Logger::instance().log(Logger::Info, "Call placed on hold");
    } catch (const pj::Error &err) {
        Logger::instance().log(Logger::Error,
            QString("Hold failed: %1").arg(err.info().c_str()));
    }
}

void SipCall::unhold()
{
    try {
        pj::CallOpParam prm;
        reinvite(prm);
        m_onHold = false;
        Logger::instance().log(Logger::Info, "Call resumed from hold");
    } catch (const pj::Error &err) {
        Logger::instance().log(Logger::Error,
            QString("Unhold failed: %1").arg(err.info().c_str()));
    }
}

void SipCall::mute(bool muted)
{
    m_muted = muted;
    // Implementation would control audio transmission
    Logger::instance().log(Logger::Info,
        QString("Call %1").arg(muted ? "muted" : "unmuted"));
}
