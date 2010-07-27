#!/bin/sh
# build.sh - build the program

make clean
source /etc/profile.d/qt3.sh
qmake3 kinvoice.pro
make
make clean