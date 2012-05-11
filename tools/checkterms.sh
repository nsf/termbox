#!/bin/bash

check() {
	TERM=$1 ./checkterm.py
}

check linux # done
check rxvt-unicode # done
check rxvt-256color # done
check xterm # done
check konsole # done
check putty
check Eterm # done (if configured with UTF-8)
check gnome # peace of shit, but works
check xfce # peace of shit, but works partially (backspace problems)
check screen # done
check xterm-256color # done
