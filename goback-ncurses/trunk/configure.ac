AC_PREREQ([2.57])

AC_INIT(goback-ncurses, 0.1, user@mail.com)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

# Check for progs
AC_PROG_CXX
AC_PROG_LIBTOOL

# Check for dependencies
PKG_CHECK_MODULES(libGoBack, [libgoback >= 0.1])

AC_OUTPUT([Makefile
    src/Makefile])
