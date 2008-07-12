bin_PROGRAMS = goback-ncurses

AM_CPPFLAGS = $(libGoBack_CFLAGS) $(NCURSES_CFLAGS)
LDADD = $(libGoBack_LIBS) $(NCURSES_LIBS) -lcurses -lopcodes -lbfd

goback_ncurses_SOURCES = \
    goback-ncurses.cpp
