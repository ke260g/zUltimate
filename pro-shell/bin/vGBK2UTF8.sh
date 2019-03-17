#!/bin/bash
if [ $# != '1' ]; then
	echo specific file
	exit -1;
fi

src=$1

if [ ! -f $src ]; then
	echo $src is not a file
	exit -1;
fi

tmp=$(mktemp)

iconv -f GBK -t UTF-8 $src > $tmp

if [ $? != '0' ]; then
	echo error occurs
	exit -1;
fi

file $src
cat $tmp > $src
rm $tmp
file $src
