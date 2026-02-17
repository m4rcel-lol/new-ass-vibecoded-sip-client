#ifndef LOGGER_H
#define LOGGER_H

// PJSIP headers define 'emit', 'signals', 'slots' macros that conflict with Qt
// Undefine them here before including Qt headers, Qt will redefine them properly
#ifdef signals
#undef signals
#endif
#ifdef slots
#undef slots
#endif
#ifdef emit
#undef emit
#endif

#include <QObject>
#include <QString>
#include <QMutex>
#include <QFile>
#include <QTextStream>

/**
 * @brief The Logger class provides thread-safe logging functionality
 */
class Logger : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
        Critical = 4
    };

    static Logger& instance();

    void initialize(const QString &logFile = QString());
    void log(LogLevel level, const QString &message);
    void setLogLevel(LogLevel level);

signals:
    void logMessage(int level, const QString &message);

private:
    Logger();
    ~Logger() override;
    Q_DISABLE_COPY(Logger)

    QString levelToString(LogLevel level) const;

    QMutex m_mutex;
    LogLevel m_logLevel;
    QFile m_logFile;
    QTextStream m_logStream;
    bool m_initialized;
};

#endif // LOGGER_H
