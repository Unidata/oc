#!/bin/sh
set -x

# Major parameters
#SERVER="https://hydro2.unidata.ucar.edu:8443"
#KEYSERVER="https://localhost:8843/"
#NOKEYSERVER="https://localhost:8443/"
#FILE="thredds/dodsC/testStandardTdsScan/1day.nc.dds"
KEYURL="${KEYSERVER}/$FILE"
NOKEYURL="${NOKEYSERVER}/$FILE"
CERT="clientcert.pem"
CERTPWD="changeit"
KEY="clientkey.pem"
KEYPWD="changeit"
REPO="c:/Users/dmh/git/thredds/cdm/src/test/resources"

BUILDSUFFIX="../oc"

OCLOGFILE=""

# if this is part of a distcheck action, then this script
# will be executed in a different directory
# than the one containing it; so capture the path to this script
# as the location of the source directory.
srcdir=`dirname $0`

# compute the build directory
# Do a hack to remove e.g. c: for CYGWIN
cd `pwd`
builddir=`pwd`/${BUILDSUFFIX}
cd $builddir
builddir=`pwd`
# Hack for CYGWIN
cd $srcdir
srcdir=`pwd`

cd ${builddir}

# Get the key and cert files
rm -f ./$CERT ./$KEY
cp $REPO/$KEY .
cp $REPO/$CERT .

# Create the .dodsrc file
rm -f ./.dodsrc
echo "HTTP.VERBOSE=1" >>.dodsrc
echo "HTTP.COOKIEJAR=./.dods_cookies" >>.dodsrc
echo "HTTP.SSL.VALIDATE=1" >>.dodsrc
#Following two lines should be same
echo "HTTP.SSL.CERTIFICATE=./${CERT}" >>.dodsrc
echo "HTTP.SSL.KEY=./${KEY}" >>.dodsrc
echo "HTTP.SSL.KEYPASSWORD=${KEYPWD}" >>.dodsrc


# Invoke octest to extract a file from the servers
# do both with and witout key
echo "=================================================="
./octest -p dds -g -L "$KEYURL"
#echo "=================================================="
#./octest -p dds -g -L "$NOKEYURL"

