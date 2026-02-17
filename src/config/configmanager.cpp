#include "configmanager.h"
#include <KConfigGroup>
#include "../utils/logger.h"

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager()
    : QObject(nullptr)
{
    m_config = KSharedConfig::openConfig("kde-sip-clientrc");
    Logger::instance().log(Logger::Info, "ConfigManager initialized");
}

ConfigManager::~ConfigManager()
{
    sync();
}

QVariant ConfigManager::value(const QString &key, const QVariant &defaultValue) const
{
    QStringList parts = key.split('/');
    if (parts.size() < 2) {
        return defaultValue;
    }

    QString groupName = parts[0];
    QString keyName = parts.mid(1).join('/');

    KConfigGroup group = m_config->group(groupName);
    return group.readEntry(keyName, defaultValue);
}

void ConfigManager::setValue(const QString &key, const QVariant &value)
{
    QStringList parts = key.split('/');
    if (parts.size() < 2) {
        return;
    }

    QString groupName = parts[0];
    QString keyName = parts.mid(1).join('/');

    KConfigGroup group = m_config->group(groupName);
    group.writeEntry(keyName, value);

    emit configChanged(key);
}

void ConfigManager::sync()
{
    m_config->sync();
}
