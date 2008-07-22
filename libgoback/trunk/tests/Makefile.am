TESTS = \
	FileDataSource-open-existing \
	FileDataSource-open-unexisting \
	WorkModeHex-numberlines-fullline \
	WorkModeHex-numberlines-halfline \
	WorkModeHex-numberlines-jumpline

EXTRA_DIST = $(TESTS)

check_PROGRAMS = \
	FileDataSource-open \
	WorkModeHex-numberlines

LDADD = ../src/libgoback.la
AM_CPPFLAGS = -I$(top_srcdir)/include/

FileDataSource_open_SOURCES = FileDataSource-open.cpp
WorkModeHex_numberlines_SOURCES = WorkModeHex-numberlines.cpp
