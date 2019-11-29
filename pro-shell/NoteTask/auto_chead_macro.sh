#!/bin/bash
ls -1 *.h | while read header; do
    macro_like=$(echo $header | tr [:lower:]. [:upper:]_)
    macro=__${macro_like}__
    # handle dos's line terminator issue
    sed -i "s|\r$||" $header

    # delete old macro
    sed -i "/$macro_like/d" $header
    sed -i "/#ifndef.*__[A-Z_]*_H__/d" $header
    sed -i "/#define.*__[A-Z_]*_H__/d" $header
    sed -i "/#endif.*__[A-Z_]*_H__/d" $header

    # insert new macro
    sed -i "1i#ifndef $macro" $header
    sed -i "1a#define $macro" $header
    sed -i "\$a#endif \/\/ $macro" $header
done