from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import glob
import platform

sourcefiles = ['src/termboxmodule.pyx'] + \
	list(filter(lambda n: n != "src/termboxmodule.c", glob.glob('src/*.c')))

with open('src/termboxconfig.pyx', 'w') as f:
    f.write("DEF PY_MAJOR_VERSION = %s\n" % platform.python_version()[0])

setup(
    name = 'termbox',
    version = '1.0.1a1',
    description = 'A simple and clean ncurses alternative',
    author = 'nsf',
    author_email = 'no.smile.face@gmail.com',
    url = 'http://code.google.com/p/termbox/',
    license = 'MIT',
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension('termbox', sourcefiles)],
)
