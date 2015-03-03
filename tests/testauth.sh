#!/bin/sh
#set -x

NOEMBED=1
NOLOCAL=1
NOHOME=1
NOSPEC=1
#NOENV=1

SHOW=1
#DBG=1
GDB=1

NFL=1

WD=`pwd`

NETRCFILE=$WD/test_auth_netrc
# This is the control variable
NETRC=$NETRCFILE

COOKIES="${WD}/test_auth_cookies"

RC=.ocrc

OCLOGFILE=stderr
if test "x$DBG" = x1 ; then
SHOW=1
fi

# Major parameters

BASICCOMBO="tiggeUser:tigge"
URLSERVER="remotetest.unidata.ucar.edu"
URLPATH="thredds/dodsC/restrict/testData.nc"

# See if we need to override
if test "x$URS" != "x" ; then
#https://54.86.135.31/opendap/data/nc/fnoc1.nc.dds
URLSERVER="54.86.135.31"
URLPATH="opendap/data/nc/fnoc1.nc"
BASICCOMBO="$URS"
NOEMBED=1
NETRC=$NETRCFILE
else
NETRC=
fi

# Split the combo
BASICUSER=`echo $BASICCOMBO | cut -d: -f1`
BASICPWD=`echo $BASICCOMBO | cut -d: -f2`

OCPRINT=
for o in ../ocprint.exe ./ocprint.exe ../ocprint ./ocprint ; do
  if test -f $o ; then
  OCPRINT=$o
  break;
  fi
done
if test "x$OCPRINT" = x ; then
echo "no ocprint"
exit 1
fi

if test "x$SHOW" = x ; then
OUTPUT="-o /dev/null"
else
OUTPUT=
fi

if test "x$TEMP" = x ; then
  TEMP="/tmp"
fi

LOCALRC=./$RC
HOMERC=${HOME}/$RC
HOMERC=`echo "$HOMERC" | sed -e "s|//|/|g"`
SPECRC="$TEMP/temprc"
ENVRC="$TEMP/envrc"

cd `pwd`
builddir=`pwd`
# Hack for CYGWIN
cd $srcdir
srcdir=`pwd`
cd ${builddir}

function createrc {
if test "x$1" != x ; then
RCP=$1

rm -f $RCP
echo "Creating rc file $RCP"
if test "x${DBG}" != x ; then
echo "HTTP.VERBOSE=1" >>$RCP
fi	
echo "HTTP.COOKIEJAR=${COOKIES}" >>$RCP
if test "x${URS}" = x ; then
echo "HTTP.CREDENTIALS.USERPASSWORD=${BASICCOMBO}" >>$RCP
fi
if test "x${NETRC}" != x && test "x$NFL" = x ; then
echo "HTTP.NETRC=${NETRC}" >>$RCP
fi
fi
}

function createnetrc {
if test "x$1" != x ; then
rm -f $1
echo "Creating netrc file $1"
echo "machine uat.urs.earthdata.nasa.gov login $BASICUSER password $BASICPWD" >>$1
#echo "machine 54.86.135.31 login $BASICUSER password $BASICPWD" >>$1
fi
}

function reset {
  for f in ./$RC $HOME/$RC $SPECRC $ENVRC $COOKIES $NETRC ; do
    rm -f ${f}
  done      
}

function restore {
  reset
  for f in ./$RC $HOME/$RC $SPECRC $ENVRC $COOKIES $NETRC ; do
    if test -f ${f}.save ; then
      echo "restoring old ${f}"
      cp ${f}.save ${f}
    fi      
  done      
}

function save {
  for f in ./$RC $HOME/$RC $SPECRC $ENVRC $COOKIES $NETRC ; do
    if test -f $f ; then
      if test -f ${f}.save ; then
        ignore=1
      else
        echo "saving $f"
        cp ${f} ${f}.save
      fi
    fi      
  done      
}

# Assemble the ocprint command
if test "x$DBG" = x1; then
OCPRINT="$OCPRINT -D1"
fi

if test "x$NETRC" != x ; then
if test "x$NFL" = x1 ; then
OCPRINT="$OCPRINT -N $NETRC"
fi
fi

if test "x$GDB" = x1 ; then
OCPRINT="gdb --args $OCPRINT -D1"
fi

# Initialize
save
reset

if test "x$NOEMBED" != x1 ; then
echo "***Testing rc file with embedded user:pwd"
URL="https://${BASICCOMBO}@${URLSERVER}/$URLPATH"
# Invoke ocprint to extract a file the URL
echo "command: ${OCPRINT} -R NONE -p dds ${OUTPUT} $URL"
${OCPRINT} -R NONE -p dds ${OUTPUT} "$URL"
fi


URL="https://${URLSERVER}/$URLPATH"
if test "x$NOLOCAL" != x1 ; then
echo "***Testing rc file in local directory"
# Create the rc file and (optional) netrc fil in ./
reset
createnetrc $NETRC
createrc $LOCALRC

# Invoke ocprint to extract a file the URL
echo "command: ${OCPRINT} -p dds ${OUTPUT} $URL"
${OCPRINT} -p dds ${OUTPUT} "$URL"
fi

if test "x$NOHOME" != x1 ; then
echo "***Testing rc file in home directory"
# Create the rc file and (optional) netrc fil in ./
reset
createnetrc $NETRC
createrc $HOMERC

# Invoke ocprint to extract a file the URL
echo "command: ${OCPRINT} -p dds -L ${OUTPUT} $URL"
${OCPRINT} -p dds -L ${OUTPUT} "$URL"
fi

if test "x$NOSPEC" != x1 ; then
echo "*** Testing rc file in specified directory"
# Create the rc file and (optional) netrc file
reset
createnetrc $NETRC
createrc $SPECRC

# Invoke ocprint to extract a file the URL
echo "command: ${OCPRINT} -p dds -L -R $SPECRC ${OUTPUT} $URL"
${OCPRINT} -p dds -L -R $SPECRC ${OUTPUT} "$URL"
fi

if test "x$NOENV" != x1 ; then
echo "*** Testing rc file using env variable"
# Create the rc file and (optional) netrc file
reset
createnetrc $NETRC
export OCRCFILE=$ENVRC
createrc $OCRCFILE

# Invoke ocprint to extract a file the URL
echo "command: ${OCPRINT} -p dds -L ${OUTPUT} $URL"
${OCPRINT} -p dds -L ${OUTPUT} "$URL"
fi

#cleanup
restore

