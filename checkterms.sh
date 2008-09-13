#!/bin/bash

check() {
	TERM=$1 ./checkterm.py
}

check linux
check rxvt
check rxvt-unicode
check xterm
check konsole
check putty
check Eterm
check gnome
check xfce
check screen
