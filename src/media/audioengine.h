#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>
#include <QString>

/**
 * @brief The AudioEngine class manages audio devices and routing
 */
class AudioEngine : public QObject
{
    Q_OBJECT

public:
    struct AudioDevice {
        QString id;
        QString name;
        bool isInput;
        bool isDefault;
    };

    static AudioEngine& instance();

    bool initialize();
    void shutdown();

    QList<AudioDevice> inputDevices() const;
    QList<AudioDevice> outputDevices() const;

    bool setInputDevice(const QString &deviceId);
    bool setOutputDevice(const QString &deviceId);

    QString currentInputDevice() const;
    QString currentOutputDevice() const;

signals:
    void devicesChanged();
    void inputDeviceChanged(const QString &deviceId);
    void outputDeviceChanged(const QString &deviceId);

private:
    AudioEngine();
    ~AudioEngine() override;
    Q_DISABLE_COPY(AudioEngine)

    void detectDevices();

    bool m_initialized;
    QList<AudioDevice> m_devices;
    QString m_currentInputDevice;
    QString m_currentOutputDevice;
};

#endif // AUDIOENGINE_H
