#!/bin/sh

./_make.sh

test -x ./mkdata || exit

./mkdata

test `uname` = "Linux" && OPT='-r'

test -x ../qplot && ../qplot $OPT test.qplot.ini


