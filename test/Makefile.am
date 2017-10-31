ACLOCAL_AMFLAGS= ${ACLOCAL_FLAGS} -I m4

AM_CPPFLAGS = -I../src/ $(OI_EXCEPTION_CFLAGS)
AM_CXXFLAGS = --pedantic -Wall -Werror

LDADD = \
	../src/.libs/libtidm_common.a \
	-lpthread \
	$(OI_EXCEPTION_LIBS)

check_PROGRAMS= shared_mutex hist_stat time_utils

shared_mutex_SOURCES= shared-mutex.cpp

hist_stat_SOURCES = hist_stat.cpp

time_utils_SOURCES = time.cpp

TESTS=$(check_PROGRAMS)
