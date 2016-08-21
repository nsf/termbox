Termbox is a library that provides minimalistic API which allows the
programmer to write text-based user interfaces.

It is based on a very simple abstraction. The main idea is viewing terminals as
a table of fixed-size cells and input being a stream of structured
messages. Would be fair to say that the model is inspired by windows console
API. The abstraction itself is not perfect and it may create problems in certain
areas. The most sensitive ones are copy & pasting and wide characters (mostly
Chinese, Japanese, Korean (CJK) characters). When it comes to copy & pasting,
the notion of cells is not really compatible with the idea of text. And CJK
runes often require more than one cell to display them nicely. Despite the
mentioned flaws, using such a simple model brings benefits in a form of
simplicity. And KISS principle is important.

At this point one should realize, that CLI (command-line interfaces) aren't
really a thing termbox is aimed at. But rather pseudo-graphical user interfaces.



INSTALLATION
============

Termbox comes with a waf-based build scripts. In order to configure, build and
install it, do the following::

  ./waf configure --prefix=/usr                                (configure)
  ./waf                                                        (build)
  ./waf install --destdir=DESTDIR                              (install)

By default termbox will install the header file and both shared and static
libraries. If you want to install a shared library or static library alone, use
the following as an install command::

  ./waf install --targets=termbox_shared --destdir=PREFIX      (shared library)

or::

  ./waf install --targets=termbox_static --destdir=PREFIX      (static library)


PYTHON
------

In order to install the python module, use the following command (as root or
via sudo)::

  python setup.py install

for Python 3::

  python3 setup.py install



GETTING STARTED
===============

Termbox's interface only consists of 12 functions::

  tb_init() // initialization
  tb_shutdown() // shutdown

  tb_width() // width of the terminal screen
  tb_height() // height of the terminal screen

  tb_clear() // clear buffer
  tb_present() // sync internal buffer with terminal

  tb_put_cell()
  tb_change_cell()
  tb_blit() // drawing functions

  tb_select_input_mode() // change input mode
  tb_peek_event() // peek a keyboard event
  tb_poll_event() // wait for a keyboard event

See src/termbox.h header file for full detail.



LINKS
=====

- http://pecl.php.net/package/termbox - PHP Termbox wrapper
- https://github.com/nsf/termbox-go - Go pure Termbox implementation
- https://github.com/gchp/rustbox - Rust Termbox wrapper
- https://github.com/fouric/cl-termbox - Common Lisp Termbox wrapper
- https://github.com/zyedidia/termbox-d - D Termbox wrapper

----

If you want me to add your project here, send me a pull request or drop a note
via email, you can find my email below.



BUGS & QUESTIONS
================

Report bugs to the https://github.com/nsf/termbox issue tracker. Send rants
and questions to me: no.smile.face@gmail.com.



CHANGES
=======

v1.1.0:

- API: tb_width() and tb_height() are guaranteed to be negative if the termbox
  wasn't initialized.
- API: Output mode switching is now possible, adds 256-color and grayscale color
  modes.
- API: Better tb_blit() function. Thanks, Gunnar Zötl <gz@tset.de>.
- API: New tb_cell_buffer() function for direct back buffer access.
- API: Add new init function variants which allow using arbitrary file
  descriptor as a terminal.
- Improvements in input handling code.
- Calling tb_shutdown() twice is detected and results in abort() to discourage
  doing so.
- Mouse event handling is ported from termbox-go.
- Paint demo port from termbox-go to demonstrate mouse handling capabilities.
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
