#!/bin/bash
git clone https://github.com/hillwoodroc/winetricks-zh.git
git clone https://github.com/oblique/create_ap.git

# zsh build
cd ~
git clone https://github.com/robbyrussell/oh-my-zsh.git
cp oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc
mv oh-my-zsh .oh-my-zsh

# vim
mkdir -p ~/.vim/bundle
git clone https://github.com/VundleVim/Vundle.vim.git
mv Vundle.vim ~/.vim/bundle

mkdir .vim/colors
git clone https://github.com/tomasr/molokai.git
mv molokai/colors/molokai.vim .vim/colors
rm -rf molokai

# misc
git clone https://github.com/woju/make_ext4fs.git
git clone https://github.com/anestisb/android-simg2img.git
