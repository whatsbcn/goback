noinst_LTLIBRARIES = libDataFormat.la

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/include/

libDataFormat_la_LIBADD =
libDataFormat_la_SOURCES = \
	DataFormat.cpp \
	DataFormatElf.cpp \
	DataFormatElf.h \
	DataFormatExe.cpp \
	DataFormatExe.h \
	DataFormatRaw.cpp \
	DataFormatRaw.h

# DataFormatPng
if HAVE_LIBPNG
noinst_LTLIBRARIES += libDataFormatPng.la
libDataFormat_la_LIBADD += libDataFormatPng.la
libDataFormatPng_la_CPPFLAGS = $(AM_CPPFLAGS) $(libpng_CFLAGS)
libDataFormatPng_la_LDFLAGS = $(libpng_LIBS)
libDataFormatPng_la_SOURCES = \
	DataFormatPng.cpp \
	DataFormatPng.h
endif HAVE_LIBPNG
