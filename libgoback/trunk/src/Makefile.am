lib_LTLIBRARIES = libgoback.la

AM_CPPFLAGS = -I$(top_srcdir)/include/

libgoback_la_LDFLAGS = -lpng -no-undefined -version-info 0:0:0
libgoback_la_LIBADD = $(OPCODES_LIBS)
libgoback_la_SOURCES = \
	DataFormat.cpp \
	DataFormatElf.cpp \
	DataFormatElf.h \
	DataFormatPng.cpp \
	DataFormatPng.h \
	DataFormatRaw.cpp \
	DataFormatRaw.h \
	DataSource.cpp \
	DataSourceFile.cpp \
	DataSourceFile.h \
	DataSourceRange.cpp \
	DataSourceRange.h \
	ModuleManager.cpp \
	ModuleManager.h \
	WorkMode.cpp \
	WorkModeDisasm.cpp \
	WorkModeDisasm.h \
	WorkModeElf.cpp \
	WorkModeElf.h \
	WorkModeHex.cpp \
	WorkModeHex.h
