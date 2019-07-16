# install cygwin
wget https://raw.githubusercontent.com/transcode-open/apt-cyg/master/apt-cyg
# make on right-click-menu
apt-cyg install chere
chere -i -f -t mintty
/path/to/cygwin/bin/mintty.exe -i /Cygwin-Terminal.ico -

# sshd server
ssh-host-config
ssh-user-config
net start sshd
net stop sshd

# rsyncd