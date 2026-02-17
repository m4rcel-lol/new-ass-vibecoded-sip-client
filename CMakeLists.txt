cmake_minimum_required(VERSION 3.16)

project(kde-sip-client VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(ECM 5.240.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

# Qt6 components
find_package(Qt6 6.4 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickControls2
    Network
    DBus
    Sql
    Test
)

# KDE Frameworks 6
find_package(KF6 REQUIRED COMPONENTS
    Config
    CoreAddons
    I18n
    Notifications
    Wallet
    StatusNotifierItem
    KIO
    Contacts
    WidgetsAddons
)

# Kirigami
find_package(KF6Kirigami 6.0 REQUIRED)

# PJSIP
find_package(PkgConfig REQUIRED)
pkg_check_modules(PJSIP REQUIRED libpjproject)

# PipeWire
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(data)

# Enable testing
enable_testing()
add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
