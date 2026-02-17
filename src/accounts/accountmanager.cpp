#include "accountmanager.h"
#include "../config/configmanager.h"
#include "../security/credentialmanager.h"
#include "../core/sipengine.h"
#include "../utils/logger.h"

AccountManager& AccountManager::instance()
{
    static AccountManager instance;
    return instance;
}

AccountManager::AccountManager()
    : QObject(nullptr)
{
    loadAccounts();
}

AccountManager::~AccountManager()
{
    saveAccounts();
}

bool AccountManager::addAccount(const AccountInfo &info, const QString &password)
{
    // Check if account already exists
    for (const auto &acc : m_accounts) {
        if (acc.id == info.id) {
            Logger::instance().log(Logger::Warning,
                QString("Account already exists: %1").arg(info.id));
            return false;
        }
    }

    // Store password in KWallet
    if (!CredentialManager::instance().storePassword(info.id, password)) {
        Logger::instance().log(Logger::Error,
            QString("Failed to store password for account: %1").arg(info.id));
        return false;
    }

    // Add to SIP engine
    if (!SipEngine::instance().addAccount(info.sipUri, info.username,
                                         password, info.server,
                                         info.port, info.useTls)) {
        Logger::instance().log(Logger::Error,
            QString("Failed to add account to SIP engine: %1").arg(info.id));
        return false;
    }

    m_accounts.append(info);
    saveAccounts();

    emit accountAdded(info.id);
    Logger::instance().log(Logger::Info,
        QString("Account added: %1").arg(info.displayName));

    return true;
}

bool AccountManager::removeAccount(const QString &accountId)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].id == accountId) {
            // Remove from SIP engine
            SipEngine::instance().removeAccount(m_accounts[i].sipUri);

            // Remove password from KWallet
            CredentialManager::instance().removePassword(accountId);

            m_accounts.removeAt(i);
            saveAccounts();

            emit accountRemoved(accountId);
            Logger::instance().log(Logger::Info,
                QString("Account removed: %1").arg(accountId));

            return true;
        }
    }

    return false;
}

bool AccountManager::updateAccount(const AccountInfo &info)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].id == info.id) {
            m_accounts[i] = info;
            saveAccounts();

            emit accountUpdated(info.id);
            Logger::instance().log(Logger::Info,
                QString("Account updated: %1").arg(info.displayName));

            return true;
        }
    }

    return false;
}

QList<AccountInfo> AccountManager::accounts() const
{
    return m_accounts;
}

AccountInfo AccountManager::account(const QString &accountId) const
{
    for (const auto &acc : m_accounts) {
        if (acc.id == accountId) {
            return acc;
        }
    }
    return AccountInfo();
}

bool AccountManager::enableAccount(const QString &accountId, bool enable)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].id == accountId) {
            m_accounts[i].enabled = enable;
            saveAccounts();

            Logger::instance().log(Logger::Info,
                QString("Account %1: %2")
                    .arg(accountId)
                    .arg(enable ? "enabled" : "disabled"));

            return true;
        }
    }

    return false;
}

void AccountManager::loadAccounts()
{
    // Load accounts from config
    auto &config = ConfigManager::instance();

    int accountCount = config.value("Accounts/count", 0).toInt();

    for (int i = 0; i < accountCount; ++i) {
        QString prefix = QString("Accounts/account_%1/").arg(i);

        AccountInfo info;
        info.id = config.value(prefix + "id").toString();
        info.displayName = config.value(prefix + "displayName").toString();
        info.sipUri = config.value(prefix + "sipUri").toString();
        info.username = config.value(prefix + "username").toString();
        info.server = config.value(prefix + "server").toString();
        info.port = config.value(prefix + "port", 5060).toInt();
        info.useTls = config.value(prefix + "useTls", false).toBool();
        info.enabled = config.value(prefix + "enabled", true).toBool();
        info.registered = false;

        m_accounts.append(info);
    }

    Logger::instance().log(Logger::Info,
        QString("Loaded %1 accounts").arg(m_accounts.size()));
}

void AccountManager::saveAccounts()
{
    auto &config = ConfigManager::instance();

    config.setValue("Accounts/count", m_accounts.size());

    for (int i = 0; i < m_accounts.size(); ++i) {
        QString prefix = QString("Accounts/account_%1/").arg(i);
        const AccountInfo &info = m_accounts[i];

        config.setValue(prefix + "id", info.id);
        config.setValue(prefix + "displayName", info.displayName);
        config.setValue(prefix + "sipUri", info.sipUri);
        config.setValue(prefix + "username", info.username);
        config.setValue(prefix + "server", info.server);
        config.setValue(prefix + "port", info.port);
        config.setValue(prefix + "useTls", info.useTls);
        config.setValue(prefix + "enabled", info.enabled);
    }

    config.sync();
}
