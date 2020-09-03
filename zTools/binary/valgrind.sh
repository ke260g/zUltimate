export GCOV_PREFIX_STRVGCV_ADDR=6 && export VALGRIND_LIB=valgrind-$version/lib/valgrind
valgrind  --extra-debuginfo-path=/symbol/package \
    --leak-check=full \
    --undef-value-errors=no \
    --keep-debuginfo=yes \
    --log-file="exec_file.%p.val.txt" \
    exec_file 2>&1 1> exec_file.stdout