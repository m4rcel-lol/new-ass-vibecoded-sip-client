#include "dialerpage.h"
#include "../core/sipengine.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <KLocalizedString>
#include <KNotification>

DialerPage::DialerPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void DialerPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Number input
    m_numberInput = new QLineEdit(this);
    m_numberInput->setPlaceholderText(i18n("Enter number..."));
    m_numberInput->setFont(QFont("Sans", 18));
    mainLayout->addWidget(m_numberInput);

    // Dialpad grid
    QGridLayout *dialpadLayout = new QGridLayout();

    const QString digits[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#"};

    for (int i = 0; i < 12; ++i) {
        QPushButton *btn = new QPushButton(digits[i], this);
        btn->setMinimumSize(80, 60);
        connect(btn, &QPushButton::clicked, this, &DialerPage::onDigitClicked);
        dialpadLayout->addWidget(btn, i / 3, i % 3);
    }

    mainLayout->addLayout(dialpadLayout);

    // Call button
    m_callButton = new QPushButton(i18n("Call"), this);
    m_callButton->setMinimumHeight(50);
    m_callButton->setStyleSheet("background-color: #27ae60; color: white; font-size: 16px;");
    connect(m_callButton, &QPushButton::clicked, this, &DialerPage::onCallClicked);
    mainLayout->addWidget(m_callButton);

    // Backspace button
    QPushButton *backspaceBtn = new QPushButton(i18n("⌫"), this);
    connect(backspaceBtn, &QPushButton::clicked, this, &DialerPage::onBackspaceClicked);
    mainLayout->addWidget(backspaceBtn);

    mainLayout->addStretch();
}

void DialerPage::onCallClicked()
{
    QString number = m_numberInput->text().trimmed();

    if (number.isEmpty()) {
        KNotification::event("error", i18n("Error"), i18n("Please enter a number"));
        return;
    }

    // Get first available account
    auto accounts = SipEngine::instance().accounts();
    if (accounts.isEmpty()) {
        KNotification::event("error", i18n("Error"), i18n("No accounts configured"));
        return;
    }

    QString accountId = QString::fromStdString(accounts[0]->getInfo().uri);

    if (SipEngine::instance().makeCall(accountId, number)) {
        KNotification::event("call", i18n("Call"), i18n("Calling %1").arg(number));
        m_numberInput->clear();
    } else {
        KNotification::event("error", i18n("Error"), i18n("Failed to make call"));
    }
}

void DialerPage::onDigitClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        m_numberInput->setText(m_numberInput->text() + btn->text());
    }
}

void DialerPage::onBackspaceClicked()
{
    QString current = m_numberInput->text();
    if (!current.isEmpty()) {
        m_numberInput->setText(current.left(current.length() - 1));
    }
}
