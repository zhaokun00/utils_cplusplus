#!/bin/sh

if [ ! -d "./build" ]; then
	mkdir -p build
fi

cd build
rm -rf *
cmake ..
make
./main
