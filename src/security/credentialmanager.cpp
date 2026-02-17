#include "credentialmanager.h"
#include "../utils/logger.h"

CredentialManager& CredentialManager::instance()
{
    static CredentialManager instance;
    return instance;
}

CredentialManager::CredentialManager()
    : QObject(nullptr)
    , m_wallet(nullptr)
    , m_walletFolder("KDE SIP Client")
{
    initialize();
}

CredentialManager::~CredentialManager()
{
    delete m_wallet;
}

bool CredentialManager::initialize()
{
    if (m_wallet) {
        return true;
    }

    if (!KWallet::Wallet::isEnabled()) {
        Logger::instance().log(Logger::Warning, "KWallet is not enabled");
        return false;
    }

    QString walletName = KWallet::Wallet::NetworkWallet();
    m_wallet = KWallet::Wallet::openWallet(walletName, 0);

    if (!m_wallet) {
        Logger::instance().log(Logger::Error, "Failed to open KWallet");
        return false;
    }

    if (!m_wallet->hasFolder(m_walletFolder)) {
        m_wallet->createFolder(m_walletFolder);
    }

    m_wallet->setFolder(m_walletFolder);

    Logger::instance().log(Logger::Info, "CredentialManager initialized with KWallet");
    return true;
}

bool CredentialManager::storePassword(const QString &accountId, const QString &password)
{
    if (!m_wallet) {
        Logger::instance().log(Logger::Error, "KWallet not available");
        return false;
    }

    if (m_wallet->writePassword(accountId, password) == 0) {
        Logger::instance().log(Logger::Info,
            QString("Password stored for account: %1").arg(accountId));
        return true;
    }

    Logger::instance().log(Logger::Error,
        QString("Failed to store password for account: %1").arg(accountId));
    return false;
}

QString CredentialManager::retrievePassword(const QString &accountId)
{
    if (!m_wallet) {
        Logger::instance().log(Logger::Error, "KWallet not available");
        return QString();
    }

    QString password;
    if (m_wallet->readPassword(accountId, password) == 0) {
        Logger::instance().log(Logger::Info,
            QString("Password retrieved for account: %1").arg(accountId));
        return password;
    }

    Logger::instance().log(Logger::Warning,
        QString("No password found for account: %1").arg(accountId));
    return QString();
}

bool CredentialManager::removePassword(const QString &accountId)
{
    if (!m_wallet) {
        Logger::instance().log(Logger::Error, "KWallet not available");
        return false;
    }

    if (m_wallet->removeEntry(accountId) == 0) {
        Logger::instance().log(Logger::Info,
            QString("Password removed for account: %1").arg(accountId));
        return true;
    }

    return false;
}

bool CredentialManager::isAvailable() const
{
    return m_wallet != nullptr;
}
