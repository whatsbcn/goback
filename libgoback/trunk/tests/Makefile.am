TESTS = \
	FileDataSource-open-existing \
	FileDataSource-open-unexisting

EXTRA_DIST = $(TESTS)

check_PROGRAMS = \
	FileDataSource-open

LDADD = ../src/libgoback.la
AM_CPPFLAGS = -I$(top_srcdir)/include/

FileDataSource_open_SOURCES = FileDataSource-open.cpp
