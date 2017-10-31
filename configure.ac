AC_PREREQ([2.67])
AC_INIT([tidm_common], [7.2], [oi@tidm.ir])
LT_INIT
LT_LANG([C++])
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

PKG_CHECK_MODULES([TIDM_EXCEPTION], [tidm_exception >= 7.0])

# FIXME: Find a way to check against msgpack
# AC_CHECK_LIB([msgpack], [pack])
AC_CHECK_HEADERS([sys/time.h])
AC_TYPE_UINT64_T

AC_FUNC_MKTIME
AC_CHECK_FUNCS([clock_gettime localtime_r])

AC_CONFIG_FILES([Makefile
                src/Makefile
                test/Makefile
                tidm_common.pc])

AC_SUBST([TIDM_COMMON_SO_VERSION], [8:0:2])
AC_SUBST([TIDM_COMMON_API_VERSION], [7.2.0])

AC_OUTPUT
