#include "audioengine.h"
#include "pipewirebackend.h"
#include "../utils/logger.h"

AudioEngine& AudioEngine::instance()
{
    static AudioEngine instance;
    return instance;
}

AudioEngine::AudioEngine()
    : QObject(nullptr)
    , m_initialized(false)
{
}

AudioEngine::~AudioEngine()
{
    shutdown();
}

bool AudioEngine::initialize()
{
    if (m_initialized) {
        return true;
    }

    Logger::instance().log(Logger::Info, "Initializing AudioEngine with PipeWire...");

    // Initialize PipeWire backend
    if (!PipeWireBackend::instance().initialize()) {
        Logger::instance().log(Logger::Error, "Failed to initialize PipeWire backend");
        return false;
    }

    detectDevices();

    m_initialized = true;
    Logger::instance().log(Logger::Info, "AudioEngine initialized successfully");

    return true;
}

void AudioEngine::shutdown()
{
    if (!m_initialized) {
        return;
    }

    Logger::instance().log(Logger::Info, "Shutting down AudioEngine...");

    PipeWireBackend::instance().shutdown();

    m_initialized = false;
}

void AudioEngine::detectDevices()
{
    m_devices.clear();

    // Get devices from PipeWire backend
    auto pwDevices = PipeWireBackend::instance().getDevices();

    for (const auto &dev : pwDevices) {
        AudioDevice device;
        device.id = dev.id;
        device.name = dev.name;
        device.isInput = dev.isInput;
        device.isDefault = dev.isDefault;
        m_devices.append(device);

        if (dev.isDefault) {
            if (dev.isInput) {
                m_currentInputDevice = dev.id;
            } else {
                m_currentOutputDevice = dev.id;
            }
        }
    }

    Logger::instance().log(Logger::Info,
        QString("Detected %1 audio devices").arg(m_devices.size()));

    emit devicesChanged();
}

QList<AudioEngine::AudioDevice> AudioEngine::inputDevices() const
{
    QList<AudioDevice> inputs;
    for (const auto &dev : m_devices) {
        if (dev.isInput) {
            inputs.append(dev);
        }
    }
    return inputs;
}

QList<AudioEngine::AudioDevice> AudioEngine::outputDevices() const
{
    QList<AudioDevice> outputs;
    for (const auto &dev : m_devices) {
        if (!dev.isInput) {
            outputs.append(dev);
        }
    }
    return outputs;
}

bool AudioEngine::setInputDevice(const QString &deviceId)
{
    if (PipeWireBackend::instance().setInputDevice(deviceId)) {
        m_currentInputDevice = deviceId;
        emit inputDeviceChanged(deviceId);
        Logger::instance().log(Logger::Info,
            QString("Input device changed to: %1").arg(deviceId));
        return true;
    }
    return false;
}

bool AudioEngine::setOutputDevice(const QString &deviceId)
{
    if (PipeWireBackend::instance().setOutputDevice(deviceId)) {
        m_currentOutputDevice = deviceId;
        emit outputDeviceChanged(deviceId);
        Logger::instance().log(Logger::Info,
            QString("Output device changed to: %1").arg(deviceId));
        return true;
    }
    return false;
}

QString AudioEngine::currentInputDevice() const
{
    return m_currentInputDevice;
}

QString AudioEngine::currentOutputDevice() const
{
    return m_currentOutputDevice;
}
