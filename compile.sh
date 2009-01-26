#!/bin/bash

gcc -O2 -o keyboard keyboard.c termbox.c term.c input.c ringbuffer.c utf8.c
