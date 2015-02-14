#!/bin/sh

./_make.sh

test -x ./mkdatabin || exit

./mkdatabin

test `uname` = "Linux" && OPT='-r'

../../qplot $OPT binary.qplot.ini


