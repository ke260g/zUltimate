#!/bin/bash

# rm the old symbolic-link

find -name "lib*" | grep cv3 >/dev/null
if [[ `echo $?` -eq 0 ]]; then
    echo script run successfully
else

:<<! wrong version
rm -f *.so.3.1 *.so
# rename the source .so files
# [old_so.txt].so.3.1.0 to [new_so.txt]_cv3.so.3.1.0
ls *so.3.1.0 -1 > old_so.txt
cat old_so.txt| sed -e "s|\.so|_cv3.so|" > new_so.txt
diff -y old_so.txt new_so.txt |\
sed -e "s/|//"\
    -e "s| \{1,\}||"\
    -e "s|\t\{1,\}| |"\
    -e "s|^|mv |" |\
sh

# ln _cv3.so.3.1.0 to _cv3.so.3.1
cat new_so.txt > old_so.txt
sed -e "s|\.1\.0$|.1|" -i new_so.txt
diff -y old_so.txt new_so.txt |\
sed -e "s/|//"\
    -e "s| \{1,\}||"\
    -e "s|\t\{1,\}| |"\
    -e "s|^|ln -s |" |\
sh

# ln _cv3.so.3.1 to _cv3.so
cat new_so.txt > old_so.txt
sed -e "s|\.3\.1$||" -i new_so.txt
diff -y old_so.txt new_so.txt |\
sed -e "s/|//"\
    -e "s| \{1,\}||"\
    -e "s|\t\{1,\}| |"\
    -e "s|^|ln -s |" |\
sh
!

ls -1 *so > old_so.txt
cat old_so.txt > new_so.txt
sed -e "s|\.so$|_cv3.so|" -i new_so.txt
diff -y old_so.txt new_so.txt |\
sed -e "s/|//"\
    -e "s| \{1,\}||"\
    -e "s|\t\{1,\}| |"\
    -e "s|^|mv |" |\
sh


# special .a static library
mv libopencv_ts.a libopencv_ts_cv3.a
rm old_so.txt new_so.txt

mkdir cv3
mv lib* cv3
fi # rename .so

# edit new .pc
cd pkgconfig
grep cv3 opencv.pc
if [[ `echo $?` -eq 0 ]]
then
    exit
fi

sed -e "s| -lopencv_|_cv3 -lopencv_|g"\
    -e "s|lib_cv3 |lib |"\
    -e "s|-lopencv_core|-lopencv_core_cv3|"\
    -i opencv.pc
sed -e "/libdir/s|/lib$|/lib/cv3|" -i opencv.pc
sed -e "/Libs:/s|/lib|/lib/cv3|" -i opencv.pc
sed -e "/includedir/s|/include|/include/cv3|" -i opencv.pc
mv opencv.pc cv3.pc
