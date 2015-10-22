#!/bin/sh
#set -x

RCEMBED=1
RCLOCAL=1
RCHOME=1
RCSPEC=1
RCENV=1
RCPREC=1

SHOW=1
#DBG=1

# Choose one
#GDB=1
#VG=1

NFL=1

WD=`pwd`

NETRCFILE=$WD/test_auth_netrc
# This is the control variable; set when needed
unset NETRC

COOKIES="${WD}/test_auth_cookies"

RC=.daprc

OCLOGFILE=stderr
if test "x$DBG" = x1 ; then
SHOW=1
fi

# Major parameters

BASICCOMBO="tiggeUser:tigge"
BADCOMBO="tiggeUser:xxxxx"
URLSERVER="remotetest.unidata.ucar.edu"
#http://remotetest.unidata.ucar.edu/thredds/dodsC/restrict/testData.nc.html
URLPATH="thredds/dodsC/restrict/testData.nc"
PROTO=http

# See if we need to override
if test "x$URS" != "x" ; then
#https://54.86.135.31/opendap/data/nc/fnoc1.nc.dds
URLSERVER="54.86.135.31"
URLPATH="opendap/data/nc/fnoc1.nc"
BASICCOMBO="$URS"
RCEMBED=0
NETRC=$NETRCFILE
PROTO=https
fi

# Split the combo
BASICUSER=`echo $BASICCOMBO | cut -d: -f1`
BASICPWD=`echo $BASICCOMBO | cut -d: -f2`

xf() { case $- in *[x]*) set +x; XP=1;; *) XP=0;; esac }
xo() { case $XP in 1) set -x;; *) set +x;; esac }

xf
OCPRINT=
for d in "$WD/.." "$WD"; do
  for o in $d/.libs/ocprint.exe $d/.libs/ocprint $d/ocprint.exe $d/ocprint ; do
    if test -f $o ; then
      OCPRINT=$o
      break;
    fi
  done
  if test "x$OCPRINT" != x; then break; fi
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
TEMP=`echo "$TEMP" | sed -e "s|/$||"`

LOCALRC=./$RC
HOMERC=${HOME}/$RC
HOMERC=`echo "$HOMERC" | sed -e "s|//|/|g"`
SPECRC="$TEMP/temprc"
#ENVRC="$TEMP/envrc"
ENVRC="$WD/envrc"

cd `pwd`
builddir=`pwd`
# Hack for CYGWIN
cd $srcdir
srcdir=`pwd`
cd ${builddir}

createrc() {
  RCP="$1" ; shift
  unset NOPWD
  unset BADPWD
  while [[ $# > 0 ]] ; do
    case "$1" in
    nopwd) NOPWD=1 ;;
    badpwd) BADPWD=1 ;;
    *) ;;
    esac
    shift
  done
  if test "x$RCP" != x ; then
    rm -f $RCP
    echo "Creating rc file $RCP"
  else
    echo "createrc: no rc specified"
    exit 1
  fi
  if test "x${DBG}" != x ; then
    echo "HTTP.VERBOSE=1" >>$RCP
  fi	
  echo "HTTP.COOKIEJAR=${COOKIES}" >>$RCP
  if test "x${URS}" = x ; then
    if test "x${NOPWD}" = x ; then
      if test "x${BADPWD}" = x ; then
        echo "HTTP.CREDENTIALS.USERPASSWORD=${BASICCOMBO}" >>$RCP
      else
        echo "HTTP.CREDENTIALS.USERPASSWORD=${BADCOMBO}" >>$RCP
      fi
    fi
  fi
  if test "x${NETRC}" != x && test "x$NFL" = x ; then
    echo "HTTP.NETRC=${NETRC}" >>$RCP
  fi
}

createnetrc() {
  NCP="$1" ; shift
  unset NOPWD
  unset BADPWD
  while [[ $# > 0 ]] ; do
    case "$1" in
    nopwd) NOPWD=1 ;;
    badpwd) BADPWD=1 ;;
    *) ;;
    esac
    shift
  done
  if test "x$NCP" != x ; then
    rm -f $NCP
    echo "Creating netrc file $NCP"
  else
    echo "createnetrc: no rc specified"
    exit 1
  fi
  if test "x$URS" != x ; then
    echo "machine uat.urs.earthdata.nasa.gov login $BASICUSER password $BASICPWD" >>$NCP
    #echo "machine 54.86.135.31 login $BASICUSER password $BASICPWD" >>$1
  else
    echo -n "${PROTO}://$URLSERVER/$URLPATH" >>$NCP
    if test "x$NOPWD" = x ; then
      if test "x$BADPWD" = x ; then
        echo -n " login $BASICUSER password $BASICPWD" >>$NCP
      else
        echo -n " login $BASICUSER password xxxxxx" >>$NCP
      fi
    fi
    echo "" >>$NCP
  fi
}

reset() {
  for f in ./$RC $HOME/$RC $SPECRC $ENVRC $COOKIES $NETRC ; do
    rm -f ${f}
  done      
  unset DAPRCFILE
}

restore() {
  reset
  for f in ./$RC $HOME/$RC $SPECRC $ENVRC $COOKIES $NETRC ; do
    if test -f ${f}.save ; then
      echo "restoring old ${f}"
      cp ${f}.save ${f}
    fi      
  done      
}

save() {
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
  OCPRINT="gdb --args $OCPRINT"
fi
if test "x$VG" = x1 ; then
  OCPRINT="valgrind --leak-check=full $OCPRINT"
fi

# Initialize
xf
save
reset
xo

if test "x$RCEMBED" = x1 ; then
  echo "***Testing rc file with embedded user:pwd"
  URL="${PROTO}://${BASICCOMBO}@${URLSERVER}/$URLPATH"
  unset NETRC
  # Invoke ocprint to extract a file the URL
  echo "command: ${OCPRINT} -R NONE -p dds ${OUTPUT} $URL"
  ${OCPRINT} -R NONE -p dds ${OUTPUT} "$URL"
fi

# Rest of tests assume
URL="${PROTO}://${URLSERVER}/$URLPATH"
NETRC=$NETRCFILE
if test "x$RCLOCAL" = x1 ; then
  echo "***Testing rc file in local directory"
  # Create the rc file and (optional) netrc fil in ./
  xf; reset; xo
  createnetrc $NETRC
  createrc $LOCALRC

  # Invoke ocprint to extract a file using the URL
  echo "command: ${OCPRINT} -p dds ${OUTPUT} $URL"
  ${OCPRINT} -p dds ${OUTPUT} "$URL"
fi

if test "x$RCHOME" = x1 ; then
  echo "***Testing rc file in home directory"
  # Create the rc file and (optional) netrc fil in ./
  xf; reset; xo
  createnetrc $NETRC
  createrc $HOMERC

  # Invoke ocprint to extract a file the URL
  echo "command: ${OCPRINT} -p dds -L ${OUTPUT} $URL"
  ${OCPRINT} -p dds -L ${OUTPUT} "$URL"
fi

if test "x$RCSPEC" == x1 ; then
  echo "*** Testing rc file in specified directory"
  # Create the rc file and (optional) netrc file
  xf; reset; xo
  createnetrc $NETRC
  createrc $SPECRC

  # Invoke ocprint to extract a file the URL
  echo "command: ${OCPRINT} -p dds -L -R $SPECRC ${OUTPUT} $URL"
  ${OCPRINT} -p dds -L -R $SPECRC ${OUTPUT} "$URL"
fi

if test "x$RCENV" = x1 ; then
  echo "*** Testing rc file using env variable"
  # Create the rc file and (optional) netrc file
  xf; reset; xo
  createnetrc $NETRC
  echo "ENV: export DAPRCFILE=$ENVRC"
  export DAPRCFILE=$ENVRC
  createrc $DAPRCFILE

  # Invoke ocprint to extract a file the URL
  echo "command: ${OCPRINT} -p dds -L ${OUTPUT} $URL"
  ${OCPRINT} -p dds -L ${OUTPUT} "$URL"
  export DAPRCFILE=
fi

# Test that .daprc overrides netcrc for password
URL="${PROTO}://${URLSERVER}/$URLPATH"
NETRC=$NETRCFILE
if test "x$RCPREC" = x1 ; then
  echo "***Testing rc vs netrc file precedence"
  # Create the rc file and (optional) netrc file in ./
  xf; reset; xo
  createnetrc $NETRC badpwd
  createrc $LOCALRC

  # Invoke ocprint to extract a file using the URL
  echo "command: ${OCPRINT} -p dds ${OUTPUT} $URL"
  ${OCPRINT} -p dds ${OUTPUT} "$URL"
fi

xf
reset
restore
xo

exit

