lib_LTLIBRARIES = libgoback.la

AM_CPPFLAGS = -I$(top_srcdir)/include/

libgoback_la_LDFLAGS = -no-undefined -version-info 0:0:0
libgoback_la_LIBADD = $(OPCODES_LIBS)
libgoback_la_SOURCES = \
	DataSource.cpp \
	FileDataSource.cpp \
	FileDataSource.h \
	WorkMode.cpp \
	WorkModeHex.cpp \
	WorkModeHex.h \
	WorkModeElf.cpp \
	WorkModeElf.h \
	WorkModeDisasm.cpp \
	WorkModeDisasm.h
