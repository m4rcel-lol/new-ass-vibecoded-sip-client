#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QListWidget>

class HistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPage(QWidget *parent = nullptr);

private:
    void setupUI();
    void loadHistory();

    QListWidget *m_historyList;
};

#endif // HISTORYPAGE_H
