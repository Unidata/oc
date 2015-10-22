# Put together AM_CPPFLAGS and AM_LDFLAGS.

AUTOM4TE=autom4te
AM_CPPFLAGS = -I$(abs_top_srcdir)
AM_LDFLAGS = 

check_PROGRAMS =
TESTS =

EXTRA_DIST = OCTest.at $(TESTSUITE) atlocal.in $(srcdir)/package.m4 baselines testdata CMakeLists.txt testauth.sh

TESTSUITE = $(srcdir)/OCTest

check-local: atconfig atlocal $(TESTSUITE)
	$(SHELL) '$(TESTSUITE)' $(TESTSUITEFLAGS)

# If we have check-local, then installcheck-local seems redundant. With it 
# dist check runs the tests twice and there's really no difference in what's
# being tested. jhrg
# installcheck-local: atconfig atlocal $(TESTSUITE)
#	$(SHELL) '$(TESTSUITE)' AUTOTEST_PATH='$(bindir)' \
#	$(TESTSUITEFLAGS)

# For this target I added 'rm atconfig' because distcheck was complaining
# about atconfig being left in the build directory. jhrg
clean-local:
	test ! -f '$(TESTSUITE)' || $(SHELL) '$(TESTSUITE)' --clean

MAINTAINERCLEANFILES = atconfig

AUTOTEST = $(AUTOM4TE) --language=autotest
$(TESTSUITE): $(srcdir)/OCTest.at $(srcdir)/package.m4
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $@.at
	mv $@.tmp $@

# The `:;' works around a Bash 3.2 bug when the output is not writeable.
$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	:;{ \
	echo '# Signature of the current package.' && \
	echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])' && \
	echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])' && \
	echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])' && \
	echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])' && \
	echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} >'$(srcdir)/package.m4'

testurl_LDADD=$(top_builddir)/liboc.la ${CURL_LIBS}
testurl_SOURCES = testurl.c
check_PROGRAMS += testurl
TESTS += testurl

# Add some extra specialized tests
if ENABLE_AUTH_TESTS
TESTS_ENVIRONMENT=TOPSRCDIR=${abs_top_srcdir}
TESTS += testauth.sh

#t_auth_LDADD=$(top_builddir)/liboc.la ${CURL_LIBS}
#t_auth_SOURCES = t_auth.c
#check_PROGRAMS += t_auth
#TESTS += t_auth

#t_auth1_LDADD=$(top_builddir)/liboc.la ${CURL_LIBS}
#t_auth1_SOURCES = t_auth1.c
#check_PROGRAMS += t_auth1
#TESTS += t_auth1
endif

