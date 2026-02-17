# Architecture Documentation

## Overview

KDE SIP Client follows a modular, event-driven architecture with clear separation between the UI layer and the SIP/media processing layers.

## Core Components

### 1. SIP Engine (src/core/)

**Purpose**: Manages all SIP protocol operations using PJSIP library.

**Key Classes**:
- `SipEngine`: Singleton managing PJSIP lifecycle and operations
- `SipAccount`: Represents a SIP account with registration
- `SipCall`: Represents an active call

**Threading**: Runs in a separate worker thread to avoid blocking the UI.

**Responsibilities**:
- SIP registration and authentication
- Call setup and teardown
- DTMF handling
- Call transfers (blind and attended)
- NAT traversal (ICE/STUN/TURN)

### 2. Media Engine (src/media/)

**Purpose**: Handles audio input/output using PipeWire.

**Key Classes**:
- `AudioEngine`: High-level audio device management
- `PipeWireBackend`: PipeWire-specific implementation

**Features**:
- Device enumeration and selection
- Live device switching during calls
- Echo cancellation (via PJSIP)
- Audio level monitoring

### 3. Account Manager (src/accounts/)

**Purpose**: Manages SIP account configurations.

**Key Classes**:
- `AccountManager`: CRUD operations for accounts
- `AccountInfo`: Account data structure

**Storage**: Uses KConfig for configuration, KWallet for credentials.

### 4. Network Manager (src/network/)

**Purpose**: Monitors network connectivity and handles network-related operations.

**Features**:
- Network state monitoring
- Public IP detection
- IPv6 support detection

### 5. Security Layer (src/security/)

**Purpose**: Manages credentials and encryption.

**Key Classes**:
- `CredentialManager`: KWallet integration for password storage

**Features**:
- Secure password storage
- TLS certificate handling
- SRTP key management

### 6. Configuration (src/config/)

**Purpose**: Application-wide settings management.

**Key Classes**:
- `ConfigManager`: KConfig-based settings storage

### 7. UI Layer (src/ui/)

**Purpose**: Qt6-based user interface.

**Key Classes**:
- `MainWindow`: Main application window with system tray
- `DialerPage`: Phone dialer interface
- `ContactsPage`: Contact list
- `HistoryPage`: Call history
- `CallPage`: Active call interface
- `AccountsPage`: Account management UI
- `SettingsPage`: Application settings

**Design**: Follows KDE HIG, uses Plasma theme system.

### 8. Utilities (src/utils/)

**Purpose**: Common utilities and helpers.

**Key Classes**:
- `Logger`: Thread-safe logging system

## Threading Model

```
Main Thread (Qt Event Loop)
├── UI Components
└── Signal/Slot connections

SIP Worker Thread
├── PJSIP event handling
├── Call state management
└── Media processing coordination

Audio Thread (PipeWire)
├── Audio capture
└── Audio playback
```

## Data Flow

### Outgoing Call

1. User enters number in `DialerPage`
2. UI emits signal to `SipEngine`
3. `SipEngine` creates `SipCall` in worker thread
4. PJSIP initiates SIP INVITE
5. `AudioEngine` starts media capture/playback
6. UI updates to show `CallPage`

### Incoming Call

1. PJSIP receives SIP INVITE
2. `SipAccount` emits `incomingCall` signal
3. Signal propagates to UI thread
4. `KNotification` displays incoming call notification
5. User accepts/rejects via UI
6. `SipEngine` processes user action
7. Media streams established if accepted

## Configuration Storage

```
~/.config/kde-sip-clientrc    # Application settings (KConfig)
~/.local/share/kde-sip-client/ # Application data
KWallet                        # Encrypted credentials
```

## Dependencies

### Required
- Qt 6.4+ (Core, GUI, Widgets, Quick, Network, Multimedia, DBus, Sql)
- KDE Frameworks 6 (Config, CoreAddons, I18n, Notifications, Wallet, StatusNotifierItem, KIO, Contacts, Kirigami)
- PJSIP 2.x
- PipeWire 0.3+

### Optional
- CardDAV support (future)
- Video support (future)

## Performance Considerations

### Memory Management
- Singleton pattern for managers prevents duplication
- Qt smart pointers for automatic cleanup
- Minimal object creation during calls

### CPU Optimization
- Separate threads prevent UI blocking
- Efficient signal/slot connections
- Lazy initialization where possible

### Network Efficiency
- Connection pooling for SIP
- Minimal keepalive traffic
- ICE for optimal media paths

## Security Architecture

### Credential Storage
- All passwords stored in KWallet (encrypted)
- No plaintext credentials in config files
- Secure memory handling for sensitive data

### Network Security
- TLS 1.2+ for SIP signaling
- SRTP for media encryption
- Certificate validation
- Optional certificate pinning

### Sandboxing
- Flatpak manifest with minimal permissions
- D-Bus activation for reduced attack surface

## Future Enhancements

- Video support (H.264, VP8)
- Screen sharing
- Call recording
- Voicemail management
- Presence/IM (SIP MESSAGE)
- Conference calling (multi-party)
- KRunner integration
- Plasma widget
