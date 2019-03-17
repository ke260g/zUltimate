#!/bin/bash
if [ $# != '1' ]; then
	echo please specific file
	exit -1;
fi

src=$1

sed -i -e "s|\r*$||" $src
