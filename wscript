APPNAME = 'termbox'
VERSION = '1.0.0'

top = '.'
out = 'build'

import sys

def options(opt):
	opt.load('gnu_dirs')
	opt.load('compiler_c')
	opt.add_option(
		'--debug',
		action = 'store_true',
		default = False,
		help = 'Enable debug build',
	)

def configure(conf):
	conf.env.VERSION = VERSION
	conf.load('gnu_dirs')
	conf.load('compiler_c')
	conf.env.append_unique('CFLAGS', ['-std=gnu99', '-Wall', '-Wextra', '-D_XOPEN_SOURCE'])
	if conf.options.debug:
		conf.env.append_unique('CFLAGS', ['-g', '-Og'])
	else:
		conf.env.append_unique('CFLAGS', '-O3')

def build(bld):
	bld.recurse('src')
