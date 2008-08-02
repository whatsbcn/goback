SUBDIRS = include src tests

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgoback.pc

EXTRA_DIST = libgoback.doxy libgoback.pc.in

doc:
	doxygen libgoback.doxy
