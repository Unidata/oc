#!/bin/sh
#set -x

#NOLOCAL=1
#NOHOME=1
#NOSPEC=1

#DBG="-D1"

# Major parameters
BASICUSER=XXXXXXXXXXXXXXXXX
BASICPWD=XXXXXXXX
BASICCOMBO="$BASICUSER:$BASICPWD"

#https://54.172.97.47/opendap/data/nc/fnoc1.nc
URLSERVER="54.172.97.47"
#URLSERVER="uat.urs.earthdata.nasa.gov"
URLPATH="opendap/data/nc/fnoc1.nc"

COOKIES=./cookies

OCPRINT=./ocprint.exe
RC=.ocrc

OCLOGFILE=""

LOCALRC=./$RC
HOMERC=${HOME}/$RC
HOMERC=`echo "$HOMERC" | sed -e "s|//|/|g"`

SPECRC=$TEMP
if test "x$TEMP" = x ; then
  SPECRC="/tmp"
fi
SPECRC="$SPECRC/temprc"

cd `pwd`
builddir=`pwd`
# Hack for CYGWIN
cd $srcdir
srcdir=`pwd`
cd ${builddir}

URL="https://${URLSERVER}/$URLPATH"

function createrc {
RCP=$1
rm -f $RCP
if test "x${DBG}" != x ; then
echo "HTTP.VERBOSE=1" >>$RCP
fi	
echo "HTTP.COOKIEJAR=${COOKIES}" >>$RCP
#echo "HTTP.SSL.VALIDATE=1" >>$RCP
echo "HTTP.CREDENTIALS.USERPASSWORD=${BASICUSER}:${BASICPWD}" >>$RCP
}

if test "x$NOLOCAL" != x1 ; then
echo "***Testing rc file in local directory"
# 1. Create the rc file in ./
rm -f $HOMERC $LOCALRC $SPECRC $COOKIES
createrc $LOCALRC
# Invoke ocprint to extract a file the URL
${OCPRINT} $DBG -p dds "$URL"
fi

if test "x$NOHOME" != x1 ; then
echo "***Testing rc file in home directory"
# 1. Create the rc file in $HOME
rm -f $HOMERC $LOCALRC $SPECRC $COOKIES
createrc $HOMERC
# Invoke ocprint to extract a file the URL
${OCPRINT} $DBG -p dds -g -L "$URL"
fi

if test "x$NOSPEC" != x1 ; then
echo "*** Testing rc file in specified directory"
# 1. Create the rc file in $SPECRC
rm -f $SPECRC $HOMERC $LOCALRC $COOKIES
createrc $SPECRC
# Invoke ocprint to extract a file the URL
${OCPRINT} $DBG -p dds -g -L -R $SPECRC "$URL"
fi

#cleanup
rm -f $LOCALRC
rm -f $HOMERC
rm -f $SPECRC
rm -f $COOKIES

#
# URS
#[https://uat.urs.earthdata.nasa.gov]HTTP.CREDENTIALS.USER=ndp_opendap
