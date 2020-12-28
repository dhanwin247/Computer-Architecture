AC_PREREQ([2.69])
AC_INIT([uPowerSim], [1.0])

AC_CONFIG_SRCDIR([src/uPowerSim.c])
AC_CONFIG_AUX_DIR([build-aux])

AC_SEARCH_LIBS([pow], [m])

dnl Initialize Automake
AM_INIT_AUTOMAKE([subdir-objects 1.15 foreign])

AC_PROG_CC
AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT