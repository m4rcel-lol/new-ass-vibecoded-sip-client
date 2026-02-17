#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

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
#include <QVariant>
#include <KSharedConfig>

/**
 * @brief The ConfigManager class manages application configuration using KConfig
 */
class ConfigManager : public QObject
{
    Q_OBJECT

public:
    static ConfigManager& instance();

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);

    void sync();

signals:
    void configChanged(const QString &key);

private:
    ConfigManager();
    ~ConfigManager() override;
    Q_DISABLE_COPY(ConfigManager)

    KSharedConfigPtr m_config;
};

#endif // CONFIGMANAGER_H
