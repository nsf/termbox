APPNAME = 'termbox'
VERSION = '1.1'

top = '.'
out = 'build'

import sys

def options(opt):
	opt.load('compiler_c')

def configure(conf):
	conf.load('compiler_c')
	conf.env.append_unique('CFLAGS', ['-Wall', '-Wextra', '-g', '-O0'])

def build(bld):
	bld.recurse('src')
