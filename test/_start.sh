#!/bin/sh

./_make.sh

test -x ./mkdata || exit

./mkdata

test `uname` = "Linux" && OPT='-r'

test -x ../qplot && ../qplot $OPT test1.qplot.ini

test -x ../qplot && ../qplot $OPT test2.qplot.ini

