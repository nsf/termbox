#!/bin/bash

gcc -lstdc++ -O2 -o keyboard keyboard.cpp termbox.c term.c input.c ringbuffer.c
