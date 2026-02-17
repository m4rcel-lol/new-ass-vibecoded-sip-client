#include "accountspage.h"
#include "../accounts/accountmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <KLocalizedString>

AccountsPage::AccountsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    refreshAccounts();
}

void AccountsPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel(i18n("SIP Accounts"), this);
    title->setFont(QFont("Sans", 16, QFont::Bold));
    layout->addWidget(title);

    m_accountsList = new QListWidget(this);
    layout->addWidget(m_accountsList);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_addButton = new QPushButton(i18n("Add Account"), this);
    connect(m_addButton, &QPushButton::clicked, this, &AccountsPage::onAddAccountClicked);
    buttonLayout->addWidget(m_addButton);

    m_removeButton = new QPushButton(i18n("Remove Account"), this);
    connect(m_removeButton, &QPushButton::clicked, this, &AccountsPage::onRemoveAccountClicked);
    buttonLayout->addWidget(m_removeButton);

    layout->addLayout(buttonLayout);
}

void AccountsPage::onAddAccountClicked()
{
    // Show add account dialog
    // For now, just a placeholder
}

void AccountsPage::onRemoveAccountClicked()
{
    // Remove selected account
}

void AccountsPage::refreshAccounts()
{
    m_accountsList->clear();

    auto accounts = AccountManager::instance().accounts();
    if (accounts.isEmpty()) {
        m_accountsList->addItem(i18n("No accounts configured"));
    } else {
        for (const auto &account : accounts) {
            QString status = account.registered ? i18n("Registered") : i18n("Not registered");
            m_accountsList->addItem(QString("%1 - %2").arg(account.displayName).arg(status));
        }
    }
}
