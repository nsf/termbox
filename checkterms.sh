#!/bin/bash

check() {
	TERM=$1 ./checkterm.py
}

check linux # done
check rxvt
check rxvt-unicode # done
check xterm # done
check konsole # uses xterm
check putty
check Eterm # done (if configured with UTF-8)
check gnome # peace of shit
check xfce # peace of shit
check screen # done
