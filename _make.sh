#!/bin/sh

test `uname` = "Linux" && OPT='-j4' || OPT='WIN32=1'

#mkdir -p _bin_release
#mkdir -p _bin_debug
qmake
make $OPT

