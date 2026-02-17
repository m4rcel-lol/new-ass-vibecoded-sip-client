#include "networkmanager.h"
#include "../utils/logger.h"
#include <QNetworkInterface>

NetworkManager& NetworkManager::instance()
{
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager()
    : QObject(nullptr)
    , m_online(false)
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::initialize()
{
    Logger::instance().log(Logger::Info, "Initializing NetworkManager...");
    detectNetworkState();
    return true;
}

bool NetworkManager::isOnline() const
{
    return m_online;
}

QString NetworkManager::publicIpAddress() const
{
    return m_publicIp;
}

bool NetworkManager::hasIpv6() const
{
    for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                    return true;
                }
            }
        }
    }
    return false;
}

void NetworkManager::detectNetworkState()
{
    bool wasOnline = m_online;
    m_online = false;

    for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            m_online = true;
            break;
        }
    }

    if (wasOnline != m_online) {
        Logger::instance().log(Logger::Info,
            QString("Network state changed: %1").arg(m_online ? "online" : "offline"));
        emit networkStateChanged(m_online);
    }
}
