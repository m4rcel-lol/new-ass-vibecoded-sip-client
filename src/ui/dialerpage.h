#ifndef DIALERPAGE_H
#define DIALERPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class DialerPage : public QWidget
{
    Q_OBJECT

public:
    explicit DialerPage(QWidget *parent = nullptr);

private slots:
    void onCallClicked();
    void onDigitClicked();
    void onBackspaceClicked();

private:
    void setupUI();

    QLineEdit *m_numberInput;
    QPushButton *m_callButton;
};

#endif // DIALERPAGE_H
