#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// PJSIP headers may define 'emit', 'signals', 'slots' macros that conflict with Qt
#ifdef signals
#undef signals
#endif
#ifdef slots
#undef slots
#endif
#ifdef emit
#undef emit
#endif

#include <QMainWindow>
#include <QStackedWidget>
#include <KStatusNotifierItem>

class QSystemTrayIcon;

/**
 * @brief The MainWindow class is the main application window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showDialer();
    void showContacts();
    void showHistory();
    void showAccounts();
    void showSettings();
    void toggleWindow();

private:
    void setupUI();
    void setupActions();
    void setupTrayIcon();
    void setupConnections();

    QStackedWidget *m_stackedWidget;
    KStatusNotifierItem *m_trayIcon;

    // Pages
    QWidget *m_dialerPage;
    QWidget *m_contactsPage;
    QWidget *m_historyPage;
    QWidget *m_accountsPage;
    QWidget *m_settingsPage;
};

#endif // MAINWINDOW_H
