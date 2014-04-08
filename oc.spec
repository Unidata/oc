
Name: oc
Summary: The OPeNDAP C DAP2 library (client-side only)
Version: 2.0
Release: 1

Source0: ftp://ftp.unidata.ucar.edu/pub/opendap/source/oc-%{version}.tar.gz
URL: http://www.opendap.org/

Group: Development/Libraries
BuildRoot:  %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
License: LGPL/W3C
BuildRequires: curl-devel >= 7.10.6

# This package could be relocatable. In that case uncomment the following
# line
Prefix: %{_prefix}

%description
The OPeNDAP C API (OC) provides a set of C-language type definitions and
functions that can be used to retrieve data over the internet from servers
that implement the OPeNDAP Data Access Protocol (DAP). The OC
implementation provides a low-overhead way to retrieve this data, making it
practical to include OPeNDAP capability in relatively simple software. Use
the oc to build clients such as command extensions for IDL and Matlab
which are implemented as shared object libraries.

%package devel
Summary: Static libraries and header files from Oc
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Requires: curl-devel >= 7.10.6
#
%description devel
This package contains all the files needed to develop applications that
will use the OPeNDAP C API (oc).

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
rm -f $RPM_BUILD_ROOT/%{_libdir}/*.la

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_bindir}/ocprint
%{_libdir}/liboc.so*
%doc README RELEASE_NOTES COPYING INSTALL 

%files devel
%defattr(-,root,root,-)
%{_libdir}/liboc.a
%{_bindir}/oc-config
%{_includedir}/oc/

%changelog
* Mon Jul 01 2009 Rikki McQueary <rmcqueary@opendap.org>
- Initial build.

