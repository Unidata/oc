# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.62)
AC_INIT(oc, 4.0.0, support@opendap.org)
AC_CONFIG_AUX_DIR(conf)
AC_CONFIG_MACRO_DIR([conf])
AM_INIT_AUTOMAKE([foreign dist-zip subdir-objects])

# Set up libtool.
AC_MSG_NOTICE([setting up libtool])
#LT_PREREQ([2.2])
#LT_INIT(win32-dll)
LT_INIT

AC_CANONICAL_HOST
AC_SUBST(host)

AC_CONFIG_HEADERS([config.h])

dnl library versionning: Update these when the interface changes. Generally,
dnl assume that the interface tracks the major and minor release numbers.
LIB_MAJOR=4
LIB_MINOR=0
LIB_PATCH=0
AC_SUBST(LIB_MAJOR)
AC_SUBST(LIB_MINOR)
AC_SUBST(LIB_PATCH)

LIB_NOTES=""
#LIB_NOTES=":RC1"

LIBOC_VERSION="$LIB_MAJOR:$LIB_MINOR:$LIB_PATCH:${LIB_NOTES}"
AC_SUBST(LIBOC_VERSION)

# Checks for programs.
AC_PROG_CC
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
AC_CHECK_FUNCS([memset memmove strcasecmp strchr strrchr strdup strerror strtol access unlink mkstemp getpid strndup strncpy])

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
# Control various subdirs.

AC_ARG_ENABLE([dap2],
              [AS_HELP_STRING([--enable-dap2],
                                 [enable dap2 support])])
test "x$enable_dap2" = "xyes" || enable_dap2=no
AC_MSG_RESULT($enable_dap2)
AM_CONDITIONAL(ENABLE_DAP2, [test "x$enable_dap2" = "xyes"])

AC_ARG_ENABLE([dap4],
              [AS_HELP_STRING([--enable-dap4],
                                 [enable dap4 support])])
test "x$enable_dap4" = "xyes" || enable_dap4=no
AC_MSG_RESULT($enable_dap4)
AM_CONDITIONAL(ENABLE_DAP4, [test "x$enable_dap4" = "xyes"])

AC_ARG_ENABLE([tools],
              [AS_HELP_STRING([--enable-tools],
                                 [enable tools support])])
test "x$enable_tools" = "xyes" || enable_tools=no
AC_MSG_RESULT($enable_tools)
AM_CONDITIONAL(ENABLE_TOOLS, [test "x$enable_tools" = "xyes"])

AC_ARG_ENABLE([tests],
              [AS_HELP_STRING([--enable-tests],
                                 [enable tests support])])
test "x$enable_tests" = "xyes" || enable_tests=no
AC_MSG_RESULT($enable_tests)
AM_CONDITIONAL(ENABLE_TESTS, [test "x$enable_tests" = "xyes"])

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
AH_BOTTOM(
[/* Define strcasecmp, snprintf on Win32 systems. */
#ifdef _WIN32
#define strcasecmp _stricmp
#define snprintf _snprintf
#endif])

##################################################
# Process the Makefile.am (and other) files
AC_CONFIG_FILES([Makefile
		common/Makefile
		liboc/Makefile
		dap2/Makefile
		dap4/Makefile
		tools/Makefile
                ])
AC_CONFIG_TESTDIR(tests, [.])
AC_CONFIG_FILES([tests/Makefile tests/atlocal])
AC_CONFIG_FILES([oc-config], [chmod +x oc-config])
AC_OUTPUT
