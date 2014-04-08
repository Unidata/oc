#! /bin/sh
#
# This forms the basis for the nc-config utility, which tells you
# various things about the oc installation. This code was
# contributed by oc user Arlindo DaSilva. Thanks Arlindo!

prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_PREFIX@/lib
includedir=@CMAKE_INSTALL_PREFIX@/include

cc="@CMAKE_C_COMPILER@"
cflags="-I@CMAKE_INSTALL_PREFIX@/include @CMAKE_C_FLAGS@ @CMAKE_CPP_FLAGS@"
libs="-L@CMAKE_INSTALL_PREFIX@/lib -l@TLL_LIBS@"

version="@PACKAGE@ @VERSION@"

usage()
{
    cat <<EOF
Usage: oc-config [OPTION]

Available values for OPTION include:

  --help        display this help message and exit
  --all         display all options
  --cc          C compiler
  --cflags      pre-processor and compiler flags
  --libs        library linking information for oc
  --prefix      Install prefix
  --includedir  Include directory
  --version     Library version

EOF
    exit $1
}

all()
{
        echo
        echo "This $version has been built with the following features: "
        echo
        echo "  --cc        -> $cc"
        echo "  --cflags    -> $cflags"
        echo "  --libs      -> $libs"
        echo
        echo "  --prefix    -> $prefix"
        echo "  --includedir-> $includedir"
        echo "  --version   -> $version"
        echo
}

if test $# -eq 0; then
    usage 1
fi

while test $# -gt 0; do
    case "$1" in
    # this deals with options in the style
    # --option=value and extracts the value part
    # [not currently used]
    -*=*) value=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
    *) value= ;;
    esac

    case "$1" in

    --help)
	usage 0
	;;

    --all)
	all
	;;

    --cc)
	echo $cc
	;;

    --cflags)
	echo $cflags
	;;

     --libs)
       	echo $libs
       	;;

    --prefix)
       	echo "${prefix}"
       	;;

    --includedir)
       	echo "${includedir}"
       	;;

    --version)
	echo $version
	;;

    *)
        echo "unknown option: $1"
	usage
	exit 1
	;;
    esac
    shift
done

exit 0
