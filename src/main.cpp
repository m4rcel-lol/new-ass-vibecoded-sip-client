#include <QApplication>
#include <QCommandLineParser>
#include <KLocalizedString>
#include <KAboutData>
#include "ui/mainwindow.h"
#include "core/sipengine.h"
#include "utils/logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kde-sip-client");

    KAboutData aboutData(
        QStringLiteral("kde-sip-client"),
        i18n("KDE SIP Client"),
        QStringLiteral("1.0.0"),
        i18n("A modern SIP softphone for KDE Plasma"),
        KAboutLicense::GPL_V3,
        i18n("(c) 2026"),
        QString(),
        QStringLiteral("https://github.com/m4rcel-lol/new-ass-vibecoded-sip-client")
    );

    aboutData.addAuthor(
        i18n("KDE SIP Client Team"),
        i18n("Developer"),
        QStringLiteral("dev@example.com")
    );

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption daemonOption(
        QStringList() << "d" << "daemon",
        i18n("Run in daemon mode (background)")
    );
    parser.addOption(daemonOption);

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    // Initialize logger
    Logger::instance().initialize();
    Logger::instance().log(Logger::Info, "KDE SIP Client starting...");

    // Initialize SIP engine
    SipEngine::instance().initialize();

    // Create main window unless daemon mode
    if (!parser.isSet(daemonOption)) {
        MainWindow *window = new MainWindow();
        window->show();
    } else {
        Logger::instance().log(Logger::Info, "Running in daemon mode");
    }

    int result = app.exec();

    // Cleanup
    SipEngine::instance().shutdown();
    Logger::instance().log(Logger::Info, "KDE SIP Client shutting down");

    return result;
}
