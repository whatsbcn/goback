SUBDIRS = include src tests

# Documentation generation
if HAVE_DOXYGEN
doc:
	$(DOXYGEN_PATH) libgoback.doxy

clean-local:
	rm -rf doc
else !HAVE_DOXYGEN
doc:
	@echo "This target is disabled because we couldn't find doxygen when"
	@echo "configuring. Please run configure again with doxygen on your PATH."
endif !HAVE_DOXYGEN

# pkg-config information
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgoback.pc

EXTRA_DIST = libgoback.doxy libgoback.pc.in
