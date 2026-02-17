#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

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
#include <QList>
#include <QString>

/**
 * @brief Account information structure
 */
struct AccountInfo {
    QString id;
    QString displayName;
    QString sipUri;
    QString username;
    QString server;
    int port;
    bool useTls;
    bool enabled;
    bool registered;
};

/**
 * @brief The AccountManager class manages SIP accounts
 */
class AccountManager : public QObject
{
    Q_OBJECT

public:
    static AccountManager& instance();

    bool addAccount(const AccountInfo &info, const QString &password);
    bool removeAccount(const QString &accountId);
    bool updateAccount(const AccountInfo &info);

    QList<AccountInfo> accounts() const;
    AccountInfo account(const QString &accountId) const;

    bool enableAccount(const QString &accountId, bool enable);

signals:
    void accountAdded(const QString &accountId);
    void accountRemoved(const QString &accountId);
    void accountUpdated(const QString &accountId);
    void accountRegistrationChanged(const QString &accountId, bool registered);

private:
    AccountManager();
    ~AccountManager() override;
    Q_DISABLE_COPY(AccountManager)

    void loadAccounts();
    void saveAccounts();

    QList<AccountInfo> m_accounts;
};

#endif // ACCOUNTMANAGER_H
