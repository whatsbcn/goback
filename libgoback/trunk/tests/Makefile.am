TESTS = \
	DataSourceFile-open-existing \
	DataSourceFile-open-unexisting \
	WorkModeHex-numberlines-fullline \
	WorkModeHex-numberlines-halfline \
	WorkModeHex-numberlines-jumpline

EXTRA_DIST = $(TESTS)

check_PROGRAMS = \
	DataSourceFile-open \
	WorkModeHex-numberlines

LDADD = $(top_srcdir)/src/libgoback.la
AM_CPPFLAGS = -I$(top_srcdir)/include/

DataSourceFile_open_SOURCES = DataSourceFile-open.cpp
WorkModeHex_numberlines_SOURCES = WorkModeHex-numberlines.cpp
