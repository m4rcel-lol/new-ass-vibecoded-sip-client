#include "contactspage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>

ContactsPage::ContactsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadContacts();
}

void ContactsPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel(i18n("Contacts"), this);
    title->setFont(QFont("Sans", 16, QFont::Bold));
    layout->addWidget(title);

    m_contactsList = new QListWidget(this);
    layout->addWidget(m_contactsList);
}

void ContactsPage::loadContacts()
{
    // Placeholder - would load from KContacts
    m_contactsList->addItem(i18n("No contacts yet"));
}
