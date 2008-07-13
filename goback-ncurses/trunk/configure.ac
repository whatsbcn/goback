AC_PREREQ([2.57])

AC_INIT(goback-ncurses, 0.1, user@mail.com)
AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

# Check for progs
AC_PROG_CXX
AC_PROG_LIBTOOL

# Check for dependencies
PKG_CHECK_MODULES(libGoBack, [libgoback >= 0.1])

# ncurses
AC_CHECK_HEADERS([curses.h],,AC_MSG_ERROR(Couldn't find the required curses.h))
AC_CHECK_LIB([ncurses], [initscr], NCURSES_LIBS="-lncurses",
	AC_CHECK_LIB([curses], [initscr], NCURSES_LIBS="-lcurses",
	AC_MSG_ERROR(Unable to find libncurses or libcurses)))
AC_SUBST(NCURSES_LIBS)

AC_OUTPUT([Makefile
    src/Makefile])
