#include "settingspage.h"
#include "../media/audioengine.h"
#include "../config/configmanager.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <KLocalizedString>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadSettings();
}

void SettingsPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel(i18n("Settings"), this);
    title->setFont(QFont("Sans", 16, QFont::Bold));
    mainLayout->addWidget(title);

    // Audio settings
    QGroupBox *audioGroup = new QGroupBox(i18n("Audio Settings"), this);
    QFormLayout *audioLayout = new QFormLayout(audioGroup);

    m_inputDeviceCombo = new QComboBox(this);
    connect(m_inputDeviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsPage::onInputDeviceChanged);
    audioLayout->addRow(i18n("Input Device:"), m_inputDeviceCombo);

    m_outputDeviceCombo = new QComboBox(this);
    connect(m_outputDeviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsPage::onOutputDeviceChanged);
    audioLayout->addRow(i18n("Output Device:"), m_outputDeviceCombo);

    mainLayout->addWidget(audioGroup);

    // General settings
    QGroupBox *generalGroup = new QGroupBox(i18n("General Settings"), this);
    QVBoxLayout *generalLayout = new QVBoxLayout(generalGroup);

    m_autoAnswerCheck = new QCheckBox(i18n("Auto-answer calls"), this);
    generalLayout->addWidget(m_autoAnswerCheck);

    m_startMinimizedCheck = new QCheckBox(i18n("Start minimized to tray"), this);
    generalLayout->addWidget(m_startMinimizedCheck);

    mainLayout->addWidget(generalGroup);

    mainLayout->addStretch();

    // Load audio devices
    auto inputDevices = AudioEngine::instance().inputDevices();
    for (const auto &dev : inputDevices) {
        m_inputDeviceCombo->addItem(dev.name, dev.id);
    }

    auto outputDevices = AudioEngine::instance().outputDevices();
    for (const auto &dev : outputDevices) {
        m_outputDeviceCombo->addItem(dev.name, dev.id);
    }
}

void SettingsPage::loadSettings()
{
    auto &config = ConfigManager::instance();

    m_autoAnswerCheck->setChecked(config.value("General/autoAnswer", false).toBool());
    m_startMinimizedCheck->setChecked(config.value("General/startMinimized", false).toBool());
}

void SettingsPage::saveSettings()
{
    auto &config = ConfigManager::instance();

    config.setValue("General/autoAnswer", m_autoAnswerCheck->isChecked());
    config.setValue("General/startMinimized", m_startMinimizedCheck->isChecked());
    config.sync();
}

void SettingsPage::onInputDeviceChanged(int index)
{
    if (index >= 0) {
        QString deviceId = m_inputDeviceCombo->itemData(index).toString();
        AudioEngine::instance().setInputDevice(deviceId);
        saveSettings();
    }
}

void SettingsPage::onOutputDeviceChanged(int index)
{
    if (index >= 0) {
        QString deviceId = m_outputDeviceCombo->itemData(index).toString();
        AudioEngine::instance().setOutputDevice(deviceId);
        saveSettings();
    }
}
