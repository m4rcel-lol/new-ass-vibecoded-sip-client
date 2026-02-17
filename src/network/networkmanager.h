#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QString>

/**
 * @brief The NetworkManager class handles network-related operations
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    static NetworkManager& instance();

    bool initialize();
    bool isOnline() const;

    QString publicIpAddress() const;
    bool hasIpv6() const;

signals:
    void networkStateChanged(bool online);
    void publicIpChanged(const QString &ip);

private:
    NetworkManager();
    ~NetworkManager() override;
    Q_DISABLE_COPY(NetworkManager)

    void detectNetworkState();

    bool m_online;
    QString m_publicIp;
};

#endif // NETWORKMANAGER_H
