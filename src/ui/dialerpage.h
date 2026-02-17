#ifndef DIALERPAGE_H
#define DIALERPAGE_H

// PJSIP headers define 'emit', 'signals', 'slots' macros that conflict with Qt
// Undefine them here before including Qt headers, Qt will redefine them properly
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
