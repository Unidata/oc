EXTRA_DIST = CMakeLists.txt ocprint.1 README install.doc mainpage.doc \
	tutorial.doc oc.css doxy.conf doxy.conf.cmake \
        ${OBSOLETE} ${GENERATED} ${TEMPLATES}

TEMPLATES = auth.html.in tutorial.html.in

GENERATED = ocauth.html octutorial.html

OBSOLETE = ocuserman.html ocdata.html ocinternals.html ocpath.html

MAINTAINERCLEANFILES = Makefile Makefile.in

# Watch out, a Windows machine (not cygwin) may not have sed
ALLDOCSED = -e 's/oc.c[ ][ ]*oc.h/*.c *.h/' -e 's/OUTPUT_DIRECTORY[ ][ \
	]*=[ ][ ]*docs/OUTPUT_DIRECTORY = alldocs/'

.PHONEY: alldocs ocdocs cleandocs auth

ocdocs: cleandocs
	doxygen doxy.conf

alldocs: cleandocs
	sed $(ALLDOCSED) < doxy.conf | doxygen -

cleandocs::
	rm -fr docs/html/*

# Build certain html documents. Note that we do not do this
# automatically because a windows machine may not have 'sed'
generate::
	rm -f ${GENERATED}
	cat auth.html.in \
	    | sed -e '/<NC/d' \
	    | sed -e 's|^<OC>||' \
	    | sed -e 's|zz|oc|g' -e 's|ZZ|OC|g' \
	    | sed -e "s|OC Version VVVV|OC Version ${LIBOC_VERSION}|" \
	    | sed -e '/<!--css-->/r./oc.css' \
	    >ocauth.html
	cat tutorial.html.in \
	    | sed -e "s|OC Version VVVV|OC Version ${LIBOC_VERSION}|" \
	    | sed -e '/<!--css-->/r./oc.css' \
	    >octutorial.html

