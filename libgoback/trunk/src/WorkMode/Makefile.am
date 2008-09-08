noinst_LTLIBRARIES = libWorkMode.la

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/include/

libWorkMode_la_LIBADD = $(OPCODES_LIBS)
libWorkMode_la_SOURCES = \
	WorkMode.cpp \
	WorkModeDisasm.cpp \
	WorkModeDisasm.h \
	WorkModeElf.cpp \
	WorkModeElf.h \
	WorkModeGroup.cpp \
	WorkModeGroup.h \
	WorkModeHex.cpp \
	WorkModeHex.h \
	WorkModeElfSymb.cpp \
	WorkModeElfSymb.h
