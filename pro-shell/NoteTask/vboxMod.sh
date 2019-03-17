#!/bin/sh
vboxModules=(vboxnetflt vboxnetadp vboxpci vboxdrv)

if [[ `whoami` != root ]]
then
    echo This script has to be run as root.
    exit 1
fi

case $1 in
load)
    /sbin/modprobe ${vboxModules[@]} || exit 1
    echo load vbox kernel modules
;;

unload)
    /sbin/modprobe -r ${vboxModules[@]} || exit 1
    echo unload vbox kernel modules
;;
reload)
$0 unload && $0 load
;;
*)
echo "usage: $0 <load | unload>"

esac
