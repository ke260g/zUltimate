# YouCompleteMe official installation instructions
~/.vim/bundle/YouCompleteMe/install.sh --clang-completer
ls ~/.vim/bundle/YouCompleteMe/third_party/ycmd/.ycm_extra_conf.py # example
# .ycm_extra_conf.py => flags=['-I/usr/include', '-I/usr/inlude/c++/6']

# YouCompleteMe On Debian With Self LLVM
mkdir ~/ycm_build && cd ~/ycm_build
cmake -G "Unix Makefiles" . ~/.vim/bundle/YouCompleteMe/third_party/ycmd/cpp -DUSE_SYSTEM_LIBCLANG=ON -DEXTERNAL_LIBCLANG_PATH=/path/to/libclang.so
make ycm_core

# YouCompleteMe On Windows With Gvim
#install cmake
#install gvim: get its python3 version
#install mingw32-64: posix-seh-rt
#download llvm-src
#download clang-src
#download python3, with correspond to gvim
# compile llvm-src/clang-src with mingw32-64:
# cmake entry setting:
#    do not use python2
#    set python3/libpython.dll.a path
#    install prefix to the same path
#    build_type with Release (this is recommanded)
# compile YouCompleteMe with mingw32-64:
# cmake entry setting:
#    cmake-source => .vim/bundle/YouCompleteMe/third_party/ycmd/cpp
#    EXTERNAL_LIBCLANG_PATH => llvm/bin/libclang.dll
#    LIBCLANG_PATH point to the dll compiled with mingw32
#    mingw32-make.exe ycm_core
#    boost maybe failed with "std::snprintf", edit it to "_snprintf"
#    build_type with Release (this is recommanded)

# YouCompleteMe On Cygwin
export LIBCLANG_PATH=/path/to/libclang.dll
cmake -G "Unix Makefiles" . ~/.vim/bundle/YouCompleteMe/third_party/ycmd/cpp -DEXTERNAL_LIBCLANG_PATH=${LIBCLANG_PATH}
cmake --build . --target ycm_core --config Release
