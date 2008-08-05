AC_PREREQ([2.57])

AC_INIT(libgoback, 0.1, user@mail.com)
AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

#AM_CONFIG_HEADER(config.h)

# Check for progs
AC_PROG_CXX
AC_PROG_LIBTOOL

# Check for dependencies

# libopcodes
AC_CHECK_HEADERS([dis-asm.h],,AC_MSG_ERROR(Couldn't find the required dis-asm.h))
#AC_CHECK_LIB([opcodes], [init_disassemble_info], OPCODES_LIBS="-lopcodes -lbfd",
#	AC_MSG_ERROR(Unable to find libopcodes))
OPCODES_LIBS="-lopcodes -lbfd"
AC_SUBST(OPCODES_LIBS)

# Check for doxygen, to be able to generate the documentation
AC_PATH_PROG(DOXYGEN_PATH, doxygen)
AM_CONDITIONAL(HAVE_DOXYGEN, test x$DOXYGEN_PATH != x)

AC_OUTPUT([Makefile
    include/Makefile
    src/Makefile
    src/DataFormat/Makefile
    src/DataSource/Makefile
    src/WorkMode/Makefile
    tests/Makefile
    libgoback.pc])
