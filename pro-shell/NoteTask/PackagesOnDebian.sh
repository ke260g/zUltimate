#!/bin/bash
exit -1
# packages on debian
# not run directly
apt remove manpages-zh

# very basic on curses
vim vim-nox exuberant-ctags
zsh ranger trash-cli htop tmux pinfo dstat glances
vsftpd sshpass openssh-sftp-server openssh-server ncftp
wireshark tcpdump nmap nmapsi4 dnsutils
clang git manpages-posix-dev
asciio graphviz
source-highlight highlight
jq pyflakes
astyle indent
cscope ctags

# advanced hacking
mkdir glibc && cd glibc && apt-src glibc-source
susv2 susv3 susv4
gdb-doc
flex-doc bison-doc

# code statistic
cloc statsvn gitstats

# gui
ghex doxygen doxygen-gui cmake-gui
alacarte fcitx-tables dmz-cursor-theme xserver-xorg-input-synaptics
guvcview gtk-recordmydesktop cheese audacious
qalculate-gtk qtiplot speechcrunch
build-essential dkms broadcom-sta-dkms linux-headers-4.9.0-4-all
tigervnc-viewer filezilla 
# install 'axe'-theme from gtk3-themes.org

# vbox
VBOX_DEB_URL="http://download.virtualbox.org/virtualbox/debian"
echo "deb ${VBOX_DEB_URL} stretch contrib" > /etc/apt/sources.list.d/virtualbox.list
apt update
apt install virtualbox-5.1
# download oracle_virtualbox_extpack.deb
# dpkg -i oracle_virtualbox_extpack.deb

# R
apt install r-base r-base-dev libatlas3-base libopenblas-base
# install 'rstudio' from its website
# download rstudio-server.deb
# download rstudio-destkop.deb
# dpkg -i rstudio-server.deb
# dpkg -i rstudio-destkop.deb

# c++ ide
qt5-default qtcreator cppreference-doc-en-qch cppman
opengl-4-man-doc opengl-4-html-doc freeglut3-dev
