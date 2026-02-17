# Contributing to KDE SIP Client

Thank you for your interest in contributing to KDE SIP Client!

## Development Setup

### Prerequisites

Ensure you have the following installed:

- CMake 3.16+
- Qt 6.4+
- KDE Frameworks 6
- PJSIP library
- PipeWire development files
- C++ compiler with C++17 support

### Building from Source

```bash
git clone https://github.com/m4rcel-lol/new-ass-vibecoded-sip-client.git
cd new-ass-vibecoded-sip-client
mkdir build
cd build
cmake ..
make
```

### Running Tests

```bash
cd build
ctest
```

## Code Style

- Follow KDE coding standards
- Use 4 spaces for indentation (no tabs)
- Maximum line length: 120 characters
- Use meaningful variable and function names
- Add comments for complex logic

## Submitting Changes

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Commit your changes (`git commit -m 'Add amazing feature'`)
7. Push to the branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

## Reporting Bugs

Please use the GitHub issue tracker to report bugs. Include:

- Operating system and version
- KDE Plasma version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Relevant logs

## Feature Requests

Feature requests are welcome! Please open an issue describing:

- The feature you'd like to see
- Why it would be useful
- Potential implementation approach

## Code of Conduct

Be respectful and constructive in all interactions.
