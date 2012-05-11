from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import glob

sourcefiles = ['src/termboxmodule.pyx'] + glob.glob('src/*.c')

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
