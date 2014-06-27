# Copyright 2009, UCAR/Unidata and OPeNDAP, Inc.
# See the COPYRIGHT file for more information.
# Build liboc and oc

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I conf

AM_YFLAGS = --defines --verbose 
# I don't think there's any C++ code here. jhrg 9/30/13
# AM_CPPFLAGS = -I$(top_srcdir) ${CURL_CFLAGS}
AM_LDFLAGS = ${CURL_LIBS}

# When building for release, remove these options
if ENABLE_DEVELOPER
AM_CFLAGS = -Wall -g3 -O0
# I removed these because they are rarely used. jhrg 9/30/13
# -ftest-coverage -fprofile-arcs -pg 

# added the following line to enforce the AM_CFLAGS value set
# above--more for debugging purposes 
# CFLAGS =
else
AM_CFLAGS = -DNDEBUG
endif

TEST_COV_FLAGS = -ftest-coverage -fprofile-arcs

SUBDIRS = . tests

bin_PROGRAMS = 

lib_LTLIBRARIES = liboc.la

bin_SCRIPTS = oc-config

liboc_la_SOURCES = config.h oc.h ocx.h oc.c \
ocbytes.h ocbytes.c \
occlientparams.h occlientparams.c \
occonstraints.h \
occurlfunctions.h occurlfunctions.c \
ocdata.h ocdata.c \
ocdatatypes.h \
ocdebug.h ocdebug.c \
ocdump.h ocdump.c \
ochttp.h ochttp.c \
ocinternal.h ocinternal.c \
oclist.h oclist.c \
oclog.h oclog.c \
ocnode.h ocnode.c \
ocrc.h ocrc.c \
ocread.h ocread.c \
ocuri.h ocuri.c \
ocutil.h ocutil.c \
dap2parselex.h dap2lex.c dap2parse.c dap2tab.h dap2tab.c \
dap2compile.h dap2compile.c \
dap4parselex.h dap4lex.c dap4parse.c dap4tab.h dap4tab.c \
dap4_perfect.c yax.h yax.c \
xxdr.h xxdr.c

liboc_la_LDFLAGS = -version-info $(LIBOC_VERSION) @LDFLAGS@

pkginclude_HEADERS = oc.h

EXTRA_DIST = COPYING COPYRIGHT RELEASE_NOTES INSTALL README
EXTRA_DIST += dap2.y dap4.y dap4.gperf
EXTRA_DIST += XGetopt.c
EXTRA_DIST += oc.spec
EXTRA_DIST += CMakeLists.txt

EXTRA_DIST += docs

if INCLUDE_WIN32_DIST
EXTRA_DIST += win32
endif

MAINTAINERCLEANFILES = Makefile.in
#CLEANFILES = *.log *.output *.stackdump *.exe imain getoc *.gcno *.gcda gmon.out

###########################################################################
# ocprint prints out the result of a request (used also for testing)

bin_PROGRAMS += ocprint
ocprint_SOURCES = ocprint.c
if ENABLE_XGETOPT
ocprint_SOURCES += XGetopt.c XGetopt.h
endif

ocprint_LDADD=$(top_builddir)/liboc.la ${CURL_LIBS}

###########################################################################
# These rule are used if someone wants to rebuild the grammar files.
# Otherwise never invoked, but records how to do it.
# BTW: note that renaming is essential because otherwise
# autoconf will forcibly delete files of the name *.tab.*

.PHONEY: parsers parser2 parser4

parsers:: parser2 parser4

# Generate dap2tab.c and dap2tab.h from dap2.y
parser2::
	rm -f dap2.tab.c dap2.tab.h	
	bison --debug -d -p dap2 dap2.y
	mv dap2.tab.c dap2tab.c; mv dap2.tab.h dap2tab.h

# Generate dap4tab.c and dap4tab.h from dap4.y
# Assumes bison version 3.0 or later
parser4::
	rm -f dap4tab.c dap4tab.h	
	bison --debug -d -p dap4 dap4.y
	sed -e 's/dap4.tab.c/dap4tab.c/'< dap4.tab.c >dap4tab.c
	sed -e 's/dap4.tab.h/dap4tab.h/'< dap4.tab.h >dap4tab.h
	rm -f dap4.tab.c dap4.tab.h	

##################################################
# These rule are used if someone wants to rebuild the perfect hash function.
# Otherwise never invoked, but records how to do it.
# Only invoke if gperf program is available
# Remove the line numbers for easier debug with gdb
perfect::
	rm -f dap4_perfect.c 
	gperf dap4.gperf | sed -e '/^[#]line/d' >dap4_perfect.c

###########################################################################

# Build linux RPMs

srpm: dist
	rpmbuild -ts $(RPM_OPTIONS) @PACKAGE@-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb $(RPM_OPTIONS) @PACKAGE@-@PACKAGE_VERSION@.tar.gz

###########################################################################

# PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker*

PACKAGEMAKER=/Developer/usr/bin/packagemaker

clean-pkg:
	-rm -rf mac_osx @PACKAGE@-@PACKAGE_VERSION@.pkg

pkg-build: clean-pkg
	./configure --prefix=$(prefix) --disable-dependency-tracking
	make clean all
	DESTDIR=`pwd`/mac_osx make install

pkg-main: pkg-build
	./OSX_Resources/update_mac_package_contents.pl README
	cat ./OSX_Resources/Info.plist.proto | \
	   sed -e "s^_PREFIX_^$(prefix)^g" \
	       -e "s^_FULL_VERSION_^@PACKAGE_VERSION@^g" \
	       -e "s^_MAJOR_VERSION_^@PACKAGE_MAJOR_VERSION@^g" \
	       -e "s^_MINOR_VERSION_^@PACKAGE_MINOR_VERSION@^g" \
	       > foo
	mv foo ./OSX_Resources/Info.plist
	${PACKAGEMAKER} --root mac_osx --id org.opendap.@PACKAGE@ \
	    --title "@PACKAGE@ @PACKAGE_VERSION@" --version @PACKAGE_VERSION@ \
	    --out @PACKAGE@-@PACKAGE_VERSION@.pkg --resources OSX_Resources

pkg-dmg: pkg-main
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@.dmg
	mkdir @PACKAGE@-@PACKAGE_VERSION@
	cp -r @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@
	cp README RELEASE_NOTES @PACKAGE@-@PACKAGE_VERSION@
	dropdmg -i --sanitize-for-servers --format zlib @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@

pkg: pkg-dmg

##########################################################################

collect-coverage-data:
	(cd test-coverage; \
         cov_dat="coverage-data-`date +%m.%d.%y`"; \
         touch $$cov_dat; \
         for f in $(ALLSRCS); do \
            echo "\n*** Coverage data for $$f ***\n" >> $$cov_dat; \
            gcov -l -o ../ $$f >> $$cov_dat; \
         done)

##########################################################################
# The following code is here to show how to generate the docs directory
# It is not normally run as part of the configure process, but ocdocs
# should probably always be run after changes to the manuals or oc.[ch].

.PHONEY: alldocs ocdocs cleandocs

ALLDOCSED = -e 's/oc.c[ ][ ]*oc.h/*.c *.h/'
ALLDOCSED += -e 's/OUTPUT_DIRECTORY[ ][ ]*=[ ][ ]*docs/OUTPUT_DIRECTORY = alldocs/'

ocdocs: cleandocs otherdocs
	doxygen ${srcdir}/doxy.conf

alldocs: cleandocs otherdocs
	sed $(ALLDOCSED) < doxy.conf | doxygen -

cleandocs::
	rm -f docs/html/*
	rm -f docs/*.html

otherdocs::
	cp ./octutorial.html ./docs

