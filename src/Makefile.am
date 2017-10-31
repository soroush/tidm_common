ACLCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AM_CPPFLAGS = \
	$(TIDM_EXCEPTION_CFLAGS)

AM_CXXFLAGS = \
	--pedantic -Wall -Werror

lib_LTLIBRARIES = libtidm_common.la
libtidm_common_la_SOURCES = \
	shared_mutex.cpp \
	semaphore.cpp \
    string_formatter.cpp \
	time_utils.cpp \
    stat_info.cpp \
	string_stubs.cpp \
    scheduler.cpp \
    tracer.cpp \
    time_tracker.cpp \
	binary_semaphore.cpp

libtidm_common_la_LDFLAGS = -version-number $(TIDM_COMMON_SO_VERSION)
libtidm_common_la_LIBADD = \
	$(TIDM_EXCEPTION_LIBS)

tidm_common_includedir = $(includedir)/tidm/utils/common
tidm_common_include_HEADERS = \
	shared_mutex.hpp \
	semaphore.hpp \
    string_formatter.hpp \
	scheduler.hpp \
	prettyprint.hpp \
	prettyprint98.hpp \
	time_utils.hpp \
	time_tracker.hpp \
    stat_info.hpp \
    string_stubs.hpp \
    historical_stat.hpp \
    historical_stat.tpp \
    tracer.hpp \
	binary_semaphore.hpp
