#!/bin/bash
# This is script need to run in root
# It also need a running network-manager daemon

# get the interface wlan*
:<< ! manually define 
# IFNAME=`sudo ifconfig | grep wlan | head -1 | awk '{print $1}'`
# SSID="13220"
# PSK="gdut3220"
!

# check the program nmcli whether exits
# hash nmcli &> /dev/null || echo "you need to install network-manager" ; exit 1

# obtain through the command line parameters
if [ $# -ne 3 ]
then
	echo "Usage: $0 <interface> <ssid> <psk>"
	exit 1
fi

IFNAME=$1
SSID=$2
PSK=$3

CON_NAME=${SSID}_${IFNAME}
nmcli con del ${CON_NAME}
nmcli con add type wifi con-name ${CON_NAME} ifname ${IFNAME} ssid ${SSID}
nmcli con modify ${CON_NAME} wifi-sec.key-mgmt wpa-psk
nmcli con modify ${CON_NAME} wifi-sec.psk ${PSK}
nmcli con up ${CON_NAME}