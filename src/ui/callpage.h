#ifndef CALLPAGE_H
#define CALLPAGE_H

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
#include <QLabel>
#include <QPushButton>

class CallPage : public QWidget
{
    Q_OBJECT

public:
    explicit CallPage(QWidget *parent = nullptr);

private slots:
    void onMuteClicked();
    void onHoldClicked();
    void onHangupClicked();
    void onTransferClicked();

private:
    void setupUI();

    QLabel *m_callerLabel;
    QLabel *m_statusLabel;
    QLabel *m_durationLabel;
    QPushButton *m_muteButton;
    QPushButton *m_holdButton;
    QPushButton *m_hangupButton;
    QPushButton *m_transferButton;

    bool m_muted;
    bool m_onHold;
};

#endif // CALLPAGE_H
