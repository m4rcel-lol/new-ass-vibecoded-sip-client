#ifndef CONTACTSPAGE_H
#define CONTACTSPAGE_H

#include <QWidget>
#include <QListWidget>

class ContactsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsPage(QWidget *parent = nullptr);

private:
    void setupUI();
    void loadContacts();

    QListWidget *m_contactsList;
};

#endif // CONTACTSPAGE_H
