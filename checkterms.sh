#!/bin/bash

check() {
	TERM=$1 ./checkterm.py
}

check linux # done
check rxvt
check rxvt-unicode # done
check xterm # done
check konsole # dip
check putty
check Eterm
check gnome # fail
check xfce
check screen # done
