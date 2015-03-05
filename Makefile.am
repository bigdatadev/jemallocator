# For using custom m4 macros
ACLOCAL_AMFLAGS = -I m4

# Compiler build flags
AM_CPPFLAGS = -I${top_srcdir}/src ${JEMALLOC_CFLAGS}

# Build rules for jemallocator library
pkginclude_HEADERS = \
	src/jemallocator/jemallocator.hpp

#lib_LTLIBRARIES = \
#	src/jemallocator/libjemallocator.la

#src_jemallocator_libjemallocator_la_SOURCES = \
#	src/jemallocator/client.hpp    \
#	src/jemallocator/client.cpp    \
#	src/jemallocator/event.hpp     \
#	src/jemallocator/event.cpp     \
#	src/jemallocator/message.hpp   \
#	src/jemallocator/message.cpp   \
#	src/jemallocator/attribute.hpp \
#	src/jemallocator/attribute.cpp \
#	src/jemallocator/query.hpp     \
#	src/jemallocator/query.cpp     \
#	src/jemallocator/exception.hpp
#src_jemallocator_libjemallocator_la_LDFLAGS = -version-info 0:0:0
#src_jemallocator_libjemallocator_la_LIBADD = $(JEMALLOC_LIBS)

# Build rules for pkgconfig
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = jemallocator.pc

# Build rules for tests.
# TESTS_ENVIRONMENT: Set environment variables for the test run
# TESTS: Define programs run automatically by "make check"
# UNIT_TESTS: Define programs run automatically by "make check-unit-tests"
# check_PROGRAMS: Define programs built by "make check" but not necessarily run
TESTS_ENVIRONMENT=
TESTS=
UNIT_TESTS=
check_PROGRAMS=

# Define unit tests with gtest
UNIT_TESTS += test/jemallocator_test
check_PROGRAMS += test/jemallocator_test
test_jemallocator_test_SOURCES = \
	test/jemallocator_test.cpp \
	test/basic_test.cpp \
	test/container_test.cpp \
	test/policy_test.cpp
test_jemallocator_test_CFLAGS = -I$(top_srcdir)/src $(JEMALLOC_CFLAGS)
test_jemallocator_test_LDADD = $(JEMALLOC_LIBS) $(LDADD)

TESTS += $(UNIT_TESTS)

TESTS_ENVIRONMENT_WITH_VALGRIND="libtool --mode=execute valgrind --leak-check=full"

.PHONY: check-with-valgrind
check-with-valgrind:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT=$(TESTS_ENVIRONMENT_WITH_VALGRIND)

.PHONY: check-unit-tests
check-unit-tests:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS=$(UNIT_TESTS)

.PHONY: check-unit-tests-with-valgrind
check-unit-tests-with-valgrind:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS=$(UNIT_TESTS) TESTS_ENVIRONMENT=$(TESTS_ENVIRONMENT_WITH_VALGRIND)

# Nonstandard package files for distribution
EXTRA_DIST = \
	README.md \
	LICENSE \
	autogen.sh \
	jemallocator.pc

include Makefile.am.coverage
