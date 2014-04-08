# Configure macros for OC
# Patrice Dumas 2005 based on freetype2.m4 from Marcelo Magallon 2001-10-26, 
# based on gtk.m4 by Owen Taylor

# AC_CHECK_OC([MINIMUM-VERSION [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
# Test for oc and define oc_CFLAGS and oc_LIBS.
# Check that the version is above MINIMUM-VERSION 
# use when linking with a c++ aware linker, with a c linker you may also
# need -lstdc++

# Example use:
# AC_CHECK_oc([3.5.0],
#  [
#   LIBS="$LIBS $oc_LIBS"
#   CPPFLAGS="$CPPFLAGS $oc_CFLAGS"
#  ],
#  [ AC_MSG_ERROR([Cannot find oc])
# ])


AC_DEFUN([AC_CHECK_OC],
[
  AC_PATH_PROG([OC_CONFIG], [oc-config], [no])
  oc_min_version=m4_if([$1], [], [1.0], [$1])
  AC_MSG_CHECKING([for OC version >= $oc_min_version])
  oc_no=""
  if test "$oc_CONFIG" = "no" ; then
     oc_no=yes
  else
     oc_config_major_version=`$OC_CONFIG --version | sed 's/^liboc \([[0-9]]\)*\.\([[0-9]]*\)$/\1/'`
     oc_config_minor_version=`$OC_CONFIG --version | sed 's/^liboc \([[0-9]]\)*\.\([[0-9]]*\)$/\2/'`
     oc_min_major_version=`echo $oc_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\)/\1/'`
     oc_min_minor_version=`echo $oc_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\)/\2/'`

     oc_config_is_lt=""
     if test $oc_config_major_version -lt $oc_min_major_version ; then
       oc_config_is_lt=yes
     else
       if test $oc_config_major_version -eq $oc_min_major_version ; then
         if test $oc_config_minor_version -lt $oc_min_minor_version ; then
           oc_config_is_lt=yes
         fi
       fi
     fi
    if test x$oc_config_is_lt = xyes ; then
       oc_no=yes
     else
       OC_LIBS="`$OC_CONFIG --libs`"
       OC_CFLAGS="`$OC_CONFIG --cflags`"
     fi
   fi
   if test x$oc_no = x ; then
     AC_MSG_RESULT([yes])
     m4_if([$2], [], [:], [$2])
   else
     AC_MSG_RESULT([no])
     if test "$OC_CONFIG" = "no" ; then
     AC_MSG_NOTICE([The oc-config script could not be found.])
     else
       if test x$oc_config_is_lt = xyes ; then
         AC_MSG_NOTICE([the installed oc library is too old.])
       fi
     fi
     OC_LIBS=""
     OC_CFLAGS=""
     m4_if([$3], [], [:], [$3])
   fi
   AC_SUBST([OC_CFLAGS])
   AC_SUBST([OC_LIBS])]) 

