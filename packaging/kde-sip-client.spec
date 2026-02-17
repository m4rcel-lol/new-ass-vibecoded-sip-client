Name:           kde-sip-client
Version:        1.0.0
Release:        1%{?dist}
Summary:        Modern SIP softphone for KDE Plasma

License:        GPL-3.0-or-later
URL:            https://github.com/m4rcel-lol/new-ass-vibecoded-sip-client
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.16
BuildRequires:  gcc-c++
BuildRequires:  extra-cmake-modules
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtdeclarative-devel
BuildRequires:  qt6-qtmultimedia-devel
BuildRequires:  kf6-kconfig-devel
BuildRequires:  kf6-kcoreaddons-devel
BuildRequires:  kf6-ki18n-devel
BuildRequires:  kf6-knotifications-devel
BuildRequires:  kf6-kwallet-devel
BuildRequires:  kf6-kstatusnotifieritem-devel
BuildRequires:  kf6-kio-devel
BuildRequires:  kf6-kcontacts-devel
BuildRequires:  kf6-kirigami-devel
BuildRequires:  pjproject-devel
BuildRequires:  pipewire-devel
BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib

Requires:       qt6-qtbase
Requires:       qt6-qtdeclarative
Requires:       qt6-qtmultimedia
Requires:       kf6-kconfig
Requires:       kf6-kcoreaddons
Requires:       kf6-ki18n
Requires:       kf6-knotifications
Requires:       kf6-kwallet
Requires:       kf6-kstatusnotifieritem
Requires:       kf6-kio
Requires:       kf6-kcontacts
Requires:       kf6-kirigami
Requires:       pipewire

%description
KDE SIP Client is a modern, high-performance SIP softphone built specifically
for Fedora Linux running KDE Plasma. It provides native KDE integration with
PipeWire audio, KWallet credential storage, and follows KDE HIG.

%prep
%autosetup

%build
%cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DKDE_INSTALL_USE_QT_SYS_PATHS=ON
%cmake_build

%install
%cmake_install

desktop-file-validate %{buildroot}%{_datadir}/applications/org.kde.sipclient.desktop
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/org.kde.sipclient.appdata.xml

%files
%license LICENSE
%doc README.md
%{_bindir}/kde-sip-client
%{_datadir}/applications/org.kde.sipclient.desktop
%{_metainfodir}/org.kde.sipclient.appdata.xml

%changelog
* Tue Feb 17 2026 KDE SIP Client Team <dev@example.com> - 1.0.0-1
- Initial release
