AC_PREREQ([2.57])

AC_INIT(goback-ncurses, 0.1, user@mail.com)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

#AM_CONFIG_HEADER(config.h)

# Check for progs
AC_PROG_CXX
AC_PROG_LIBTOOL

# Check for dependencies
PKG_CHECK_MODULES(LIBGOBACK, [libgoback >= 0.1])
AC_SUBST(LIBGOBACK_CFLAGS)
AC_SUBST(LIBGOBACK_LIBS)

AC_OUTPUT([Makefile
    src/Makefile])
