SUBDIRS = DataFormat DataSource WorkMode

lib_LTLIBRARIES = libgoback.la

AM_CPPFLAGS = -I$(top_srcdir)/include/

libgoback_la_LDFLAGS = -no-undefined -version-info 0:0:0
libgoback_la_LIBADD = \
	DataFormat/libDataFormat.la \
	DataSource/libDataSource.la \
	WorkMode/libWorkMode.la
libgoback_la_SOURCES = \
	ModuleManager.cpp \
	ModuleManager.h
