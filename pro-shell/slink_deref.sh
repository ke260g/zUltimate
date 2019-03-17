#!/bin/bash
WORKFILE=`mktemp`
find . -maxdepth 1 -type l > ${WORKFILE}
sed -i -e "s|^.*$|cp \`readlink &\` &.new \&\& mv -f &.new &|" ${WORKFILE}
cat ${WORKFILE} | sh -
rm ${WORKFILE}