#include "mainwindow.h"
#include "dialerpage.h"
#include "contactspage.h"
#include "historypage.h"
#include "accountspage.h"
#include "settingspage.h"
#include "../utils/logger.h"

#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QCloseEvent>
#include <KLocalizedString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_trayIcon(nullptr)
{
    setWindowTitle(i18n("KDE SIP Client"));
    resize(800, 600);

    setupUI();
    setupActions();
    setupTrayIcon();
    setupConnections();

    Logger::instance().log(Logger::Info, "MainWindow created");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create pages
    m_dialerPage = new DialerPage(this);
    m_contactsPage = new ContactsPage(this);
    m_historyPage = new HistoryPage(this);
    m_accountsPage = new AccountsPage(this);
    m_settingsPage = new SettingsPage(this);

    // Add pages to stacked widget
    m_stackedWidget->addWidget(m_dialerPage);
    m_stackedWidget->addWidget(m_contactsPage);
    m_stackedWidget->addWidget(m_historyPage);
    m_stackedWidget->addWidget(m_accountsPage);
    m_stackedWidget->addWidget(m_settingsPage);

    setCentralWidget(m_stackedWidget);

    // Create toolbar
    QToolBar *toolbar = addToolBar(i18n("Main Toolbar"));
    toolbar->setMovable(false);

    QAction *dialerAction = toolbar->addAction(i18n("Dialer"));
    connect(dialerAction, &QAction::triggered, this, &MainWindow::showDialer);

    QAction *contactsAction = toolbar->addAction(i18n("Contacts"));
    connect(contactsAction, &QAction::triggered, this, &MainWindow::showContacts);

    QAction *historyAction = toolbar->addAction(i18n("History"));
    connect(historyAction, &QAction::triggered, this, &MainWindow::showHistory);

    QAction *accountsAction = toolbar->addAction(i18n("Accounts"));
    connect(accountsAction, &QAction::triggered, this, &MainWindow::showAccounts);

    QAction *settingsAction = toolbar->addAction(i18n("Settings"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
}

void MainWindow::setupActions()
{
    QMenu *fileMenu = menuBar()->addMenu(i18n("&File"));

    QAction *quitAction = fileMenu->addAction(i18n("&Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QMenu *viewMenu = menuBar()->addMenu(i18n("&View"));
    viewMenu->addAction(i18n("&Dialer"), this, &MainWindow::showDialer);
    viewMenu->addAction(i18n("&Contacts"), this, &MainWindow::showContacts);
    viewMenu->addAction(i18n("&History"), this, &MainWindow::showHistory);
    viewMenu->addAction(i18n("&Accounts"), this, &MainWindow::showAccounts);
    viewMenu->addSeparator();
    viewMenu->addAction(i18n("&Settings"), this, &MainWindow::showSettings);
}

void MainWindow::setupTrayIcon()
{
    m_trayIcon = new KStatusNotifierItem(this);
    m_trayIcon->setIconByName("phone");
    m_trayIcon->setTitle(i18n("KDE SIP Client"));
    m_trayIcon->setCategory(KStatusNotifierItem::Communications);
    m_trayIcon->setStatus(KStatusNotifierItem::Active);

    connect(m_trayIcon, &KStatusNotifierItem::activateRequested,
            this, &MainWindow::toggleWindow);

    Logger::instance().log(Logger::Info, "System tray icon initialized");
}

void MainWindow::setupConnections()
{
    // Setup connections between components
}

void MainWindow::showDialer()
{
    m_stackedWidget->setCurrentWidget(m_dialerPage);
}

void MainWindow::showContacts()
{
    m_stackedWidget->setCurrentWidget(m_contactsPage);
}

void MainWindow::showHistory()
{
    m_stackedWidget->setCurrentWidget(m_historyPage);
}

void MainWindow::showAccounts()
{
    m_stackedWidget->setCurrentWidget(m_accountsPage);
}

void MainWindow::showSettings()
{
    m_stackedWidget->setCurrentWidget(m_settingsPage);
}

void MainWindow::toggleWindow()
{
    if (isVisible()) {
        hide();
    } else {
        show();
        raise();
        activateWindow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon && m_trayIcon->status() == KStatusNotifierItem::Active) {
        hide();
        event->ignore();
        Logger::instance().log(Logger::Info, "Window minimized to tray");
    } else {
        event->accept();
    }
}
