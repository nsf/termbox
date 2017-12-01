AC_PREREQ(2.52)

m4_define([termbox_version_major], [1])
m4_define([termbox_version_minor], [0])
m4_define([termbox_version_patch], [0])

m4_define([termbox_version],
          [termbox_version_major.termbox_version_minor.termbox_version_patch])

AC_INIT([termbox], [termbox_version], [])

AM_INIT_AUTOMAKE([1.10 no-dist-gzip dist-bzip2 tar-ustar foreign])
AM_MAINTAINER_MODE([enable])

dnl
dnl Require a C compiler; saving & restoring flags to work-around
dnl the inclusion of GCC flags -g -O2
dnl
saved_cflags="$CFLAGS"
AC_PROG_CC
CFLAGS="$saved_cflags"

AC_PROG_LIBTOOL
AC_PROG_INSTALL

dnl
dnl C compiler flags testing
dnl
saved_cflags="$CFLAGS"
AX_CHECK_COMPILE_FLAG([-std=gnu99], [ CFLAGS="$saved_cflags -std=gnu99" ])
AX_CHECK_COMPILE_FLAG([-std=c99], [ CFLAGS="$saved_cflags -std=c99" ])

AX_CHECK_COMPILE_FLAG([-Wall], [ CFLAGS="$CFLAGS -Wall" ])
AX_CHECK_COMPILE_FLAG([-Werror], [ CFLAGS="$CFLAGS -Werror" ])
AX_CHECK_COMPILE_FLAG([-Wextra], [ CFLAGS="$CFLAGS -Wextra" ])

CFLAGS="$CFLAGS -D_XOPEN_SOURCE"

dnl
dnl Expose versioning information
dnl
TERMBOX_VERSION_MAJOR=termbox_version_major
AC_SUBST(TERMBOX_VERSION_MAJOR)

TERMBOX_VERSION_MINOR=termbox_version_minor
AC_SUBST(TERMBOX_VERSION_MINOR)

TERMBOX_VERSION_PATCH=termbox_version_patch
AC_SUBST(TERMBOX_VERSION_PATCH)

dnl
dnl Generate all build files
dnl
AC_OUTPUT([
Makefile

src/Makefile
])

