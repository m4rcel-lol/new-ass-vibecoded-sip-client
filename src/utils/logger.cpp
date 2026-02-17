#include "logger.h"
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <iostream>

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
    : QObject(nullptr)
    , m_logLevel(Info)
    , m_initialized(false)
{
}

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logStream.flush();
        m_logFile.close();
    }
}

void Logger::initialize(const QString &logFile)
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized) {
        return;
    }

    QString logPath = logFile;
    if (logPath.isEmpty()) {
        QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dataLocation);
        logPath = dataLocation + "/kde-sip-client.log";
    }

    m_logFile.setFileName(logPath);
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_logStream.setDevice(&m_logFile);
        m_initialized = true;
        log(Info, QString("Logger initialized: %1").arg(logPath));
    } else {
        std::cerr << "Failed to open log file: " << logPath.toStdString() << std::endl;
    }
}

void Logger::log(LogLevel level, const QString &message)
{
    QMutexLocker locker(&m_mutex);

    if (level < m_logLevel) {
        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString logLine = QString("[%1] [%2] %3")
                        .arg(timestamp)
                        .arg(levelToString(level))
                        .arg(message);

    // Write to file
    if (m_logFile.isOpen()) {
        m_logStream << logLine << Qt::endl;
        m_logStream.flush();
    }

    // Also write to console
    std::cout << logLine.toStdString() << std::endl;

    // Emit signal for UI logging
    emit logMessage(static_cast<int>(level), message);
}

void Logger::setLogLevel(LogLevel level)
{
    QMutexLocker locker(&m_mutex);
    m_logLevel = level;
}

QString Logger::levelToString(LogLevel level) const
{
    switch (level) {
        case Debug: return "DEBUG";
        case Info: return "INFO";
        case Warning: return "WARN";
        case Error: return "ERROR";
        case Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}
