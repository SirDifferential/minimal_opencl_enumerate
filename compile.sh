#!/bin/sh
set -x
g++ -std=c++11 -Wall -I/usr/include -I/usr/local/cuda/include -L/usr/lib/x86_64-linux-gnu -lOpenCL main.cpp -o cltest
