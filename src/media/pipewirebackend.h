#ifndef PIPEWIREBACKEND_H
#define PIPEWIREBACKEND_H

#include <QObject>
#include <QList>
#include <QString>

/**
 * @brief The PipeWireBackend class interfaces with PipeWire audio system
 */
class PipeWireBackend : public QObject
{
    Q_OBJECT

public:
    struct Device {
        QString id;
        QString name;
        bool isInput;
        bool isDefault;
    };

    static PipeWireBackend& instance();

    bool initialize();
    void shutdown();

    QList<Device> getDevices() const;
    bool setInputDevice(const QString &deviceId);
    bool setOutputDevice(const QString &deviceId);

private:
    PipeWireBackend();
    ~PipeWireBackend() override;
    Q_DISABLE_COPY(PipeWireBackend)

    bool m_initialized;
    void *m_pwContext;  // pipewire context
    void *m_pwMainLoop; // pipewire main loop
};

#endif // PIPEWIREBACKEND_H
