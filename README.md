# KDE SIP Client

A modern, high-performance SIP softphone built specifically for Fedora Linux running KDE Plasma.

## Features

- **Native KDE Integration**: First-class KDE citizen using Qt 6 and KDE Frameworks 6
- **Modern UI**: Built with Kirigami for responsive, beautiful interfaces
- **PipeWire Audio**: Native PipeWire support with PulseAudio compatibility
- **Full SIP Support**: UDP/TCP/TLS, SRTP encryption, STUN/TURN/ICE, IPv4/IPv6
- **Advanced Telephony**: Call transfer, hold, waiting, conferencing, DTMF
- **Security First**: KWallet integration, TLS 1.2+, SRTP encryption
- **Performance**: <80MB idle RAM, <5% CPU during calls, <1.5s startup

## Performance Targets

### Idle
- Memory: < 80MB RAM
- CPU: < 1%

### Active Call
- CPU: < 5%
- Zero UI stutter
- No audio pops

### Startup
- < 1.5 seconds

## Architecture

The application follows a modular, event-driven architecture:

- **SIP Core**: Non-UI thread handling SIP protocol (PJSIP)
- **Media Engine**: Audio processing with PipeWire
- **Network Manager**: Connection handling and NAT traversal
- **UI Layer**: Qt6/Kirigami interface
- **Account Manager**: Multi-account support
- **Config Manager**: KConfig-based settings
- **Security Layer**: KWallet credential storage
- **Logging System**: Comprehensive logging

## Building

### Dependencies

**Fedora:**
```bash
sudo dnf install cmake gcc-c++ extra-cmake-modules \
    qt6-qtbase-devel qt6-qtdeclarative-devel \
    kf6-kconfig-devel kf6-kcoreaddons-devel kf6-ki18n-devel \
    kf6-knotifications-devel kf6-kwallet-devel kf6-kstatusnotifieritem-devel \
    kf6-kio-devel kf6-kcontacts-devel kf6-kirigami-devel \
    pjproject-devel pipewire-devel
```

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

### Running

```bash
kde-sip-client
```

## Features

### Telephony
- Blind and attended transfer
- Call waiting
- Multi-call support
- Local conference mixing
- DTMF (RFC 2833 + SIP INFO)
- Voicemail indicator (MWI)
- SIP MESSAGE support

### Contacts
- Local storage
- vCard import/export
- CSV import
- Speed dial
- KDE address book integration

### KDE Integration
- System tray (StatusNotifierItem)
- Native Plasma notifications
- D-Bus API
- KRunner plugin support
- Plasma widget support
- Background daemon mode

### Security
- Credentials stored in KWallet
- TLS 1.2+ support
- SRTP encryption
- Certificate validation
- Optional certificate pinning
- No plaintext passwords in config

## Packaging

### RPM (Fedora/COPR)
```bash
rpmbuild -ba packaging/kde-sip-client.spec
```

### Flatpak
```bash
flatpak-builder --user --install build-dir packaging/org.kde.sipclient.yml
```

## Testing

```bash
cd build
ctest
```

## License

GPL-3.0-or-later

## Contributing

Contributions welcome! Please follow KDE HIG and coding standards.
