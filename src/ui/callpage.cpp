#include "callpage.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <KLocalizedString>

CallPage::CallPage(QWidget *parent)
    : QWidget(parent)
    , m_muted(false)
    , m_onHold(false)
{
    setupUI();
}

void CallPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // Caller info
    m_callerLabel = new QLabel(i18n("Unknown"), this);
    m_callerLabel->setFont(QFont("Sans", 24, QFont::Bold));
    m_callerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_callerLabel);

    m_statusLabel = new QLabel(i18n("Calling..."), this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    m_durationLabel = new QLabel("00:00", this);
    m_durationLabel->setFont(QFont("Sans", 18));
    m_durationLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_durationLabel);

    mainLayout->addStretch();

    // Call controls
    QGridLayout *controlsLayout = new QGridLayout();

    m_muteButton = new QPushButton(i18n("Mute"), this);
    m_muteButton->setCheckable(true);
    m_muteButton->setMinimumSize(100, 60);
    connect(m_muteButton, &QPushButton::clicked, this, &CallPage::onMuteClicked);
    controlsLayout->addWidget(m_muteButton, 0, 0);

    m_holdButton = new QPushButton(i18n("Hold"), this);
    m_holdButton->setCheckable(true);
    m_holdButton->setMinimumSize(100, 60);
    connect(m_holdButton, &QPushButton::clicked, this, &CallPage::onHoldClicked);
    controlsLayout->addWidget(m_holdButton, 0, 1);

    m_transferButton = new QPushButton(i18n("Transfer"), this);
    m_transferButton->setMinimumSize(100, 60);
    connect(m_transferButton, &QPushButton::clicked, this, &CallPage::onTransferClicked);
    controlsLayout->addWidget(m_transferButton, 1, 0);

    mainLayout->addLayout(controlsLayout);

    // Hangup button
    m_hangupButton = new QPushButton(i18n("Hang Up"), this);
    m_hangupButton->setMinimumHeight(60);
    m_hangupButton->setStyleSheet("background-color: #c0392b; color: white; font-size: 16px;");
    connect(m_hangupButton, &QPushButton::clicked, this, &CallPage::onHangupClicked);
    mainLayout->addWidget(m_hangupButton);
}

void CallPage::onMuteClicked()
{
    m_muted = !m_muted;
    m_muteButton->setText(m_muted ? i18n("Unmute") : i18n("Mute"));
    // Call SipEngine::instance().muteCall()
}

void CallPage::onHoldClicked()
{
    m_onHold = !m_onHold;
    m_holdButton->setText(m_onHold ? i18n("Resume") : i18n("Hold"));
    // Call SipEngine::instance().holdCall()
}

void CallPage::onHangupClicked()
{
    // Call SipEngine::instance().hangupCall()
    // Switch back to dialer page
}

void CallPage::onTransferClicked()
{
    // Show transfer dialog
}
