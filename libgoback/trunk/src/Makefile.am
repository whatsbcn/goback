lib_LTLIBRARIES = libgoback.la

AM_CPPFLAGS = -I$(top_srcdir)/include/

libgoback_la_LDFLAGS = -no-undefined -version-info 0:0:0
libgoback_la_LIBADD = $(OPCODES_LIBS)
libgoback_la_SOURCES = \
	DataSource.cpp \
	DataSourceFile.cpp \
	DataSourceFile.h \
	DataSourceRange.cpp \
	DataSourceRange.h \
	WorkMode.cpp \
	WorkModeDisasm.cpp \
	WorkModeDisasm.h \
	WorkModeElf.cpp \
	WorkModeElf.h \
	WorkModeHex.cpp \
	WorkModeHex.h
