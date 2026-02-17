#include "historypage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>

HistoryPage::HistoryPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadHistory();
}

void HistoryPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel(i18n("Call History"), this);
    title->setFont(QFont("Sans", 16, QFont::Bold));
    layout->addWidget(title);

    m_historyList = new QListWidget(this);
    layout->addWidget(m_historyList);
}

void HistoryPage::loadHistory()
{
    // Placeholder - would load from database
    m_historyList->addItem(i18n("No call history yet"));
}
