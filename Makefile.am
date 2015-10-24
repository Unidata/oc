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

if ENABLE_DAP2
DAP2=dap2
endif

if ENABLE_DAP4
DAP4=dap4
endif

if ENABLE_TOOLS
TOOLS=tools
endif

if ENABLE_TESTS
TESTS=tests
endif

SUBDIRS = common ${DAP2| ${DAP4} ${TOOLS} ${TESTS} buildlib

bin_SCRIPTS = oc-config

# The -no-undefined is required to actually generate a .dll(.a) file
liboc_la_LDFLAGS = -version-info $(LIBOC_VERSION) @LDFLAGS@ -no-undefined

pkginclude_HEADERS = oc.h

EXTRA_DIST = COPYING COPYRIGHT RELEASE_NOTES INSTALL README.md
EXTRA_DIST += dap.y
EXTRA_DIST += XGetopt.c
EXTRA_DIST += oc.spec
EXTRA_DIST += ocprint.1
EXTRA_DIST += CMakeLists.txt
EXTRA_DIST += occurlflags.c.in
EXTRA_DIST += makeflags.sh
EXTRA_DIST += makeparser.sh

EXTRA_DIST += docs

MAINTAINERCLEANFILES = Makefile.in
#CLEANFILES = *.log *.output *.stackdump *.exe imain getoc *.gcno *.gcda gmon.out

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

ocdocs: cleandocs
	doxygen ${srcdir}/doxy.conf

alldocs: cleandocs
	sed $(ALLDOCSED) < doxy.conf | doxygen -

cleandocs::
	rm -fr docs/html/*
