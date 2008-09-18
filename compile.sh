#!/bin/bash

gcc -O2 -o keyboard keyboard.c termbox.c term.c input.c ringbuffer.c
g++ -O2 -o test test.cpp termbox.c term.c input.c ringbuffer.c
