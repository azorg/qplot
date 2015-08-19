#!/bin/sh

test `uname` = "Linux" && OPT='-j2' || OPT='WIN32=1'

#mkdir -p _bin_release
#mkdir -p _bin_debug
qmake qplot.pro
make $OPT

