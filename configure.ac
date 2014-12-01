# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.62)
AC_INIT(oc, 2.0, support@opendap.org)
AC_CONFIG_SRCDIR([oclog.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(conf)

AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([conf])
AC_CONFIG_TESTDIR(tests, [.])
AC_CANONICAL_HOST
AC_SUBST(host)

dnl library versionning: Update these when the interface changes. Generally,
dnl assume that the interface tracks the major and minor release numbers.
LIB_MAJOR=2
LIB_MINOR=0
LIB_PATCH=0
AC_SUBST(LIB_MAJOR)
AC_SUBST(LIB_MINOR)
AC_SUBST(LIB_PATCH)

LIBOC_VERSION="$LIB_MAJOR:$LIB_MINOR:$LIB_PATCH"
AC_SUBST(LIBOC_VERSION)

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_YACC

AC_CHECK_PROG(VALGRIND, valgrind, [valgrind --logfile=memcheck])

##################################################
# Check for libcurl

# Do an initial lib test for curl, but suppress the default action
AC_CHECK_LIB([curl],[curl_easy_setopt],[found_curl=yes],[found_curl=no])
if test $found_curl = yes ; then
    # Redo the check lib to actually add -lcurl
    AC_CHECK_LIB([curl], [curl_easy_setopt])
fi

if test $found_curl = no ; then
#  Find curl-config on PATH
  if curl-config --version >/dev/null 2>&1 ; then
    AC_MSG_NOTICE([checking whether a location for curl-config is in PATH... yes])

    # Capture the libs and headers lists
    curllibs=`curl-config --libs`
    curlheaders=`curl-config --cflags`
    LIBS="$LIBS $curllibs"    
    CFLAGS="$CFLAGS $curlheaders"    
    found_curl=yes
  else
    AC_MSG_NOTICE([checking whether a location for curl-config is in PATH... no])
    AC_MSG_ERROR([cannot locate libcurl or curl-config])  
  fi
fi

# These are needed for the oc-config script. jhrg 12/19/13
CURL_CFLAGS=$curlheaders
AC_SUBST([CURL_CFLAGS])
CURL_LIBS=$curllibs
AC_SUBST([CURL_LIBS])

# CURLOPT_KEYPASSWD is not defined until curl version 7.16.4
# See if defined by trying to compile a program using it
# Save/restore CFLAGS
SAVECFLAGS="$CFLAGS"
# I think the following should be $curlheaders. jhrg 12/19/13
# CFLAGS="${curl_cflags}"
CFLAGS="${curlheaders}"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
[#include "curl/curl.h"],
[[int x = CURLOPT_KEYPASSWD;]])],
                   [havekeypassword=yes],
                   [havekeypassword=no])
AC_MSG_CHECKING([whether a CURLOPT_KEYPASSWD is defined])
AC_MSG_RESULT([${havekeypassword}])
if test $havekeypassword = yes; then
  AC_DEFINE([HAVE_CURLOPT_KEYPASSWD],[1],[Is CURLOPT_KEYPASSWD defined])
fi
CFLAGS="$SAVECFLAGS"

##################################################
# Checks for additional library functions.
case "$host_os" in
*aix*);;
*)
AC_FUNC_MALLOC
AC_FUNC_ALLOCA
;;
esac
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset strcasecmp strchr strrchr strdup strerror strtol access unlink mkstemp getpid])

##################################################
# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h libintl.h malloc.h memory.h netinet/in.h stddef.h stdlib.h string.h strings.h unistd.h stdarg.h])

##################################################
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_TYPE_LONG_LONG_INT
AC_TYPE_SIZE_T
AC_TYPE_OFF_T
AC_CHECK_TYPES([ssize_t, ptrdiff_t, uchar])
AC_C_CHAR_UNSIGNED
AC_C_BIGENDIAN
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(size_t)
AC_CHECK_SIZEOF(off_t)
AC_CHECK_SIZEOF(void*)

##################################################
# Added an --enable-developer option. Without this, define NDEBUG; with it
# suppress optimiziation, add lots of symbols, run the asserts and compile
# with -Wall
AC_ARG_ENABLE([developer],
              [AS_HELP_STRING([--enable-developer],
                                 [enable developer mode])])
test "x$enable_developer" = "xyes" || enable_developer=no
AC_MSG_RESULT($enable_developer)
AM_CONDITIONAL(ENABLE_DEVELOPER, [test "x$enable_developer" = "xyes"])

##################################################
# Control use of the remote tests
AC_MSG_CHECKING([whether remote server tests should be enabled])
AC_ARG_ENABLE([remote-tests],
              [AS_HELP_STRING([--enable-remote-tests],
                                 [enable remote tests])])
test "x$enable_remote_tests" = xyes || enable_remote_tests=no
AC_MSG_RESULT($enable_remote_tests)
AM_CONDITIONAL(ENABLE_REMOTE_TESTS, [test "x$enable_remote_tests" = "xyes"])

# Enable/disable distribute windows directory (off by default)
AC_MSG_CHECKING([include win32 in the distribution (default no)])
AC_ARG_ENABLE([win32-dist],
              [AS_HELP_STRING([--enable-win32-dist],
                                 [include win32 in distribution])])
test "x$enable_win32_dist" = xyes || enable_win32_dist=no
AC_MSG_RESULT($enable_win32_dist)
AM_CONDITIONAL(INCLUDE_WIN32_DIST, [test "x$enable_win32_dist" = "xyes"])

# Control use of xgetopt
AC_MSG_CHECKING([whether XGetopt should be used])
AC_ARG_ENABLE([xgetopt],
              [AS_HELP_STRING([--enable-xgetopt],
                                 [enable use of XGetopt])])
test "x$enable_xgetopt" = xyes || enable_xgetopt=no
AC_MSG_RESULT($enable_xgetopt)
AM_CONDITIONAL(ENABLE_XGETOPT, [test "x$enable_xgetopt" = "xyes"])

##################################################
# Control authorization testing
AC_MSG_CHECKING([whether authorization tests should be enabled])
AC_ARG_ENABLE([auth-tests],
              [AS_HELP_STRING([--enable-auth-tests],
                                 [enable authorization tests])])
test "x$enable_auth_tests" = xyes || enable_auth_tests=no
AC_MSG_RESULT($enable_auth_tests)
AM_CONDITIONAL(ENABLE_AUTH_TESTS, [test "x$enable_auth_tests" = "xyes"])

##################################################
dnl autoheader macros; tack some text at the top and bottom of config_dap.h.in

#AH_TOP([#ifndef _config_h
##define _config_h])

#AH_BOTTOM([/* Shorthand for gcc's unused attribute feature */
##if defined(__GNUG__) || defined(__GNUC__)
##define not_used __attribute__ ((unused))
##define unused __attribute__ ((unused))
##else
##define not_used 
##define unused 
##endif /* __GNUG__ || __GNUC__ */
##endif /* _config_h */])

AH_BOTTOM(
[/* Define strcasecmp, snprintf on Win32 systems. */
#ifdef _WIN32
#define strcasecmp _stricmp
#define snprintf _snprintf
#endif])

##################################################
# Process the Makefile.am (and other) files
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([tests/Makefile tests/atlocal])
AC_CONFIG_FILES([oc-config], [chmod +x oc-config])
AC_OUTPUT
