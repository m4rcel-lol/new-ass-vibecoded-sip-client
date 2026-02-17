#ifndef CREDENTIALMANAGER_H
#define CREDENTIALMANAGER_H

#include <QObject>
#include <QString>
#include <KWallet>

/**
 * @brief The CredentialManager class manages credentials using KWallet
 */
class CredentialManager : public QObject
{
    Q_OBJECT

public:
    static CredentialManager& instance();

    bool initialize();

    bool storePassword(const QString &accountId, const QString &password);
    QString retrievePassword(const QString &accountId);
    bool removePassword(const QString &accountId);

    bool isAvailable() const;

private:
    CredentialManager();
    ~CredentialManager() override;
    Q_DISABLE_COPY(CredentialManager)

    KWallet::Wallet *m_wallet;
    QString m_walletFolder;
};

#endif // CREDENTIALMANAGER_H
