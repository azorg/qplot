#!/bin/sh

if [ `uname` = "Linux" ]
then
  PROC_NUM=`grep processor /proc/cpuinfo | wc -l`
  OPT="-j $PROC_NUM"
else
  OPT="WIN32=1"
fi

#mkdir -p _bin_release
#mkdir -p _bin_debug

#qmake qplot.pro QMAKE_CC=clang QMAKE_CXX=clang++
#qmake qplot.pro
qmake qplot-qwt.pro
#qmake qplot-win.pro

make $OPT

