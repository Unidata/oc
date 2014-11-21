#!/bin/sh
set -x

# Major parameters
BASICUSER=tiggeUser
BASICPWD=tigge

URLSERVER="https://remotetest.unidata.ucar.edu"
URLPATH="dodsC/restrict/testdata.nc"
URL="${URLSERVER}/thredds/$URLPATH"

OCPRINT=../ocprint
OCLOGFILE=""

rm -f {HOME}/.dodsrc ./.dodsrc
#RC=${HOME}/.dodsrc
RC=./.dodsrc

# compute the build directory
# Do a hack to remove e.g. c: for CYGWIN
builddir=`pwd`
srcdir=${builddir}

# Create the .dodsrc file

rm -f ${RC}
echo "HTTP.VERBOSE=1" >>${RC}
echo "HTTP.CREDENTIALS.USERPASSWORD=${BASICUSER}:${BASICPWD}" >> ${RC}

# Invoke ocprint to extract a file from the server
${OCPRINT} -p dds -g -L "$URL"

#rm -f ${builddir}/oc_cookies ${RC}

exit
##################################################
# Create the .dodsrc file

RC=./dodsrc

rm -f ${RC}
echo "CURL.VERBOSE=1" >>${RC}
echo "CURL.COOKIEJAR=${builddir}/oc_cookies" >>${RC}
echo "HTTP.SSL.VALIDATE=1" >>${RC}
echo "HTTP.SSL.CERTIFICATE=${srcdir}/${CERT}" >>${RC}
echo "HTTP.SSL.KEY=${srcdir}/${KEY}" >>${RC}
echo "HTTP.SSL.KEYPASSWORD=${PASSWD}" >>${RC}
#echo "HTTP.SSL.CAPATH=${srcdir}/${SVCCERTS}" >>${RC}
echo "HTTP.SSL.VERIFYPEER=0" >>${RC}


CERT="client.pem"
KEY="client.key"
CERTPWD="changeit"
SVCCERTS="cacert.pem"

# Invoke ocprint to extract a file from the server
${OCPRINT} -p dds -g -L "$URL"

#rm -f ${builddir}/oc_cookies ${RC}


