#!/bin/bash

# an example about using expect

# ssh go to 21.186 and 15.240
# and update sdk without interact

if [[ $# -ne 2 ]]; then
    echo "usage: $0 <sdk_name> <version>"
    exit
fi

# login into 15.240
# login into 21.159
SDK_NAME=$1
VERSION=$2

SSH_240="ssh packer@192.168.15.240 "
SSH_159="ssh -p 62919 packer@192.168.21.159 "
SSH_PASSWD="commonsdkpacker"
SSH_PASSWD_240=$SSH_PASSWD
SSH_PASSWD_159=$SSH_PASSWD

SdkPacks_240="repackage_platform/SdkPacks"
SdkPacks_159="repackage_platform_new/repackage_platform/SdkPacks"

/usr/bin/expect -c "
    set timeout 3
    spawn bash -c \"$SSH_240\";
    expect \"*password*\";
    send \"$SSH_PASSWD_240\n\";
    send \"svn up $SdkPacks_240/$SDK_NAME\n\";
    send \"exit\n\";
    interact;
    spawn bash -c \"$SSH_159\";
    expect \"*password*\";
    send \"$SSH_PASSWD_159\n\";
    send \"svn up $SdkPacks_159/$SDK_NAME\n\";
    send \"exit\n\";
    interact;"
