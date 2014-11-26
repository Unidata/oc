#!/bin/sh
#set -x

#NOLOCAL=1
#NOHOME=1
#NOSPEC=1

#DBG="-D1"

# Major parameters
BASICUSER=tiggeUser
BASICPWD=tigge
BASICCOMBO="$BASICUSER:$BASICPWD"

URLSERVER="remotetest.unidata.ucar.edu"
URLPATH="thredds/dodsC/restrict/testData.nc"

COOKIES=./cookies

OCPRINT=../ocprint
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
builddir=`pwd`/..
# Hack for CYGWIN
cd $srcdir
srcdir=`pwd`
cd ${builddir}/tests

URL="http://${URLSERVER}/$URLPATH"

function createrc {
RCP=$1
rm -f $RCP
if test "x${DBG}" != x ; then
echo "HTTP.VERBOSE=1" >>$RCP
fi	
echo "HTTP.COOKIEJAR=${COOKIES}" >>$RCP
echo "HTTP.SSL.VALIDATE=1" >>$RCP
echo "HTTP.CREDENTIALS.USERPASSWORD=${BASICUSER}:${BASICPWD}" >>$RCP
#echo "HTTP.SSL.CERTIFICATE=${srcdir}/${CERT}" >>$RCP
#echo "HTTP.SSL.KEY=${srcdir}/${KEY}" >>$RCP
#echo "HTTP.SSL.KEYPASSWORD=${PASSWD}" >>$RCP
#echo "HTTP.SSL.CAPATH=${srcdir}/${SVCCERTS}" >>$RCP
#echo "HTTP.SSL.VERIFYPEER=0" >>$RCP
}

if test "x$NOLOCAL" != x1 ; then
echo "***Testing rc file in local directory"
# 1. Create the rc file in ./
rm -f $HOMERC $LOCALRC $SPECRC $COOKIES
createrc $LOCALRC
# Invoke ocprint to extract a file the URL
../ocprint $DBG -p dds -g -L "$URL"
fi

if test "x$NOHOME" != x1 ; then
echo "***Testing rc file in home directory"
# 1. Create the rc file in $HOME
rm -f $HOMERC $LOCALRC $SPECRC $COOKIES
createrc $HOMERC
# Invoke ocprint to extract a file the URL
../ocprint $DBG -p dds -g -L "$URL"
fi

if test "x$NOSPEC" != x1 ; then
echo "*** Testing rc file in specified directory"
# 1. Create the rc file in $SPECRC
rm -f $SPECRC $HOMERC $LOCALRC $COOKIES
createrc $SPECRC
# Invoke ocprint to extract a file the URL
../ocprint $DBG -p dds -g -L -R $SPECRC "$URL"
fi

#cleanup
rm -f $LOCALRC
rm -f $HOMERC
rm -f $SPECRC
rm -f $COOKIES
