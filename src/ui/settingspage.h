#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);

private slots:
    void onInputDeviceChanged(int index);
    void onOutputDeviceChanged(int index);

private:
    void setupUI();
    void loadSettings();
    void saveSettings();

    QComboBox *m_inputDeviceCombo;
    QComboBox *m_outputDeviceCombo;
    QCheckBox *m_autoAnswerCheck;
    QCheckBox *m_startMinimizedCheck;
};

#endif // SETTINGSPAGE_H
