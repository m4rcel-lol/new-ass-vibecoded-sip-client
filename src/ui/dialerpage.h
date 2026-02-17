#ifndef DIALERPAGE_H
#define DIALERPAGE_H

// PJSIP headers may define 'emit', 'signals', 'slots' macros that conflict with Qt
// Only undef if QObject hasn't been included yet (to avoid removing Qt's definitions)
#ifndef QOBJECT_H
#ifdef signals
#undef signals
#endif
#ifdef slots
#undef slots
#endif
#ifdef emit
#undef emit
#endif
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
