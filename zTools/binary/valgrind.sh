export GCOV_PREFIX_STRVGCV_ADDR=6 && export VALGRIND_LIB=valgrind-$version/lib/valgrind
valgrind  --extra-debuginfo-path=/symbol/package \
    --leak-check=full \
    --undef-value-errors=no \
    --keep-debuginfo=yes \
    --log-file="${EXEC_NAME}.%p.val.txt" \
    ${EXEC_NAME} 2>&1 1> ${EXEC_NAME}.stdout


export VALGRIND_LIB=valgrind_arm64/lib/valgrind
export DEBUG_SYMBOL=/root/TS79-MSU-S7-debug-symbol
./valgrind_arm64/bin/valgrind --extra-debuginfo-path=$DEBUG_SYMBOL --leak-check=full --undef-value-errors=no --keep-debuginfo=yes --log-file="sw_wns_stkconf_cli.%p.val.txt" ./sw_wns_stkconf_cli 2>&1 1 >> sw_wns_stkconf_cli.stdout

ubuntu 下要替换两个库; 这一拷贝就不能回头了
cp ./valgrind_arm64/lib/ld-2.22.so /lib/ld-2.22.so
cp ./valgrind_arm64/lib/libc-2.22.so /lib/libc-2.22.so



