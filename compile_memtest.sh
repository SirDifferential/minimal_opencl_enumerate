#!/bin/sh
set -x
g++ -std=c++11 -Wall -I/usr/include -L/usr/lib/x86_64-linux-gnu -lOpenCL memtest.cpp openclutils.cpp -o clmemtest
