#################################################################
#   Termbox - A ncurses alternative
#
#   Author: nsf - no.smile.face@gmail.com
#   Website: http://code.google.com/p/termbox/
#   
#   Termbox library is a simple and clean ncurses alternative.
#   Of course everything has it's own price.
#   In this case it's portability and stability.
#
#   Modified by: ykj - youngkevinjonas@gmail.com
#   On: May 5th 2012
#   I have completely reorganized the project
#   while leaving most of the code intact (fixed
#   a couple of warnings). Now you can either use
#   it as a dependency for a C/C++ project by employing
#   CMake, or you can install it as a Python package
#   by using Python distutils and Cython.
#
#   This file constitutes the CMake build.
#################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(TERMBOX C)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=gnu99")

ADD_SUBDIRECTORY(src)
