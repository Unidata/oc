#!/bin/sh
set -x

# Major parameters
SERVER="https://hydro2.unidata.ucar.edu:8443"
FILE="test.01"
URL="${SERVER}/dts/$FILE"
CERT="client.pem"
KEY="client.key"
PASSWD="changeit"
SVCCERTS="cacert.pem"

OCLOGFILE=""

# if this is part of a distcheck action, then this script
# will be executed in a different directory
# than the one containing it; so capture the path to this script
# as the location of the source directory.
srcdir=`dirname $0`

# compute the build directory
# Do a hack to remove e.g. c: for CYGWIN
cd `pwd`
builddir=`pwd`/..
# Hack for CYGWIN
cd $srcdir
srcdir=`pwd`
cd ${builddir}/ncdap_test

# Create the .dodsrc file
rm -f ./.dodsrc
echo "CURL.VERBOSE=1" >>.dodsrc
echo "CURL.COOKIEJAR=${builddir}/.oc_cookies" >>.dodsrc
echo "HTTP.SSL.VALIDATE=1" >>.dodsrc
echo "HTTP.SSL.CERTIFICATE=${srcdir}/${CERT}" >>.dodsrc
echo "HTTP.SSL.KEY=${srcdir}/${KEY}" >>.dodsrc
echo "HTTP.SSL.KEYPASSWORD=${PASSWD}" >>.dodsrc
#echo "HTTP.SSL.CAPATH=${srcdir}/${SVCCERTS}" >>.dodsrc
echo "HTTP.SSL.VERIFYPEER=0" >>.dodsrc
# Invoke octest to extract a file from the
../octest -p dds -g -L "$URL"


