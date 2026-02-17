#ifndef ACCOUNTSPAGE_H
#define ACCOUNTSPAGE_H

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

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class AccountsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AccountsPage(QWidget *parent = nullptr);

private slots:
    void onAddAccountClicked();
    void onRemoveAccountClicked();
    void refreshAccounts();

private:
    void setupUI();

    QListWidget *m_accountsList;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
};

#endif // ACCOUNTSPAGE_H
