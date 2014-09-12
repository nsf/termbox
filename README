Termbox is a library that provides minimalistic API which allows the
programmer to write text-based user interfaces.

TODO: better intro.

CHANGES
=======

v1.1.0 (not released yet):
- API: tb_width() and tb_height() are guaranteed to be negative if the termbox
  wasn't initialized.
- API: Output mode switching is now possible, adds 256-color and grayscale color
  modes.
- API: Better tb_blit() function. Thanks, Gunnar Zötl <gz@tset.de>.
- Bug fixes in code and documentation.

v1.0.0:
- Remove the Go directory. People generally know about termbox-go and where
  to look for it.
- Remove old terminfo-related python scripts and backport the new one from
  termbox-go.
- Remove cmake/make-based build scripts, use waf.
- Add a simple terminfo database parser. Now termbox prefers using the
  terminfo database if it can be found. Otherwise it still has a fallback
  built-in database for most popular terminals.
- Some internal code cleanups and refactorings. The most important change is
  that termbox doesn't leak meaningless exported symbols like 'keys' and
  'funcs' now. Only the ones that have 'tb_' as a prefix are being exported.
- API: Remove unsigned ints, use plain ints instead.
- API: Rename UTF-8 functions 'utf8_*' -> 'tb_utf8_*'.
- API: TB_DEFAULT equals 0 now, it means you can use attributes alones
  assuming the default color.
- API: Add TB_REVERSE.
- API: Add TB_INPUT_CURRENT.
- Move python module to its own directory and update it due to changes in the
  termbox library.



INSTALLATION
============

Termbox comes with a waf-based build scripts. In order to configure, build and
install it, do the following:

  ./waf configure --prefix=/usr                                (configure)
  ./waf                                                        (build)
  ./waf install --destdir=DESTDIR                              (install)

By default termbox will install the header file and both shared and static
libraries. If you want to install a shared library or static library alone, use
the following as an install command:

  ./waf install --targets=termbox_shared --destdir=PREFIX      (shared library)
or
  ./waf install --targets=termbox_static --destdir=PREFIX      (static library)


PYTHON
------

In order to install the python module, use the following command (as root or
via sudo):

  python setup.py install

for Python 3:

  python3 setup.py install



API DOCUMENTATION
=================

See src/termbox.h header file.


LINKS
=====

http://pecl.php.net/package/termbox - PHP termbox wrapper

----

If you want me to add your project here, send me a pull request or drop a note
via email, you can find my email at the bottom of this file.

BUGS & QUESTIONS
================

Report bugs to the https://github.com/nsf/termbox issue tracker. Send rants
and questions to me: no.smile.face@gmail.com.
