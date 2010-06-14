#!/bin/bash

if [ ! -e "_cmake_build" ]; then
	mkdir _cmake_build
	cd _cmake_build
	cmake -DCMAKE_BUILD_TYPE=RELEASE -DTERMBOX_SHARED=ON ../..
	cd ..
fi

cd _cmake_build
make termbox
cd ..

#make
