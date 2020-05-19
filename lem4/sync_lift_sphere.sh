#!/bin/bash

LOCK_FILE="/tmp/lift_sphere.lock"
HOME_SSID="NSA_VAN"
ACTUAL_SSID=`/sbin/iwgetid -r`

touch testing.really

if [ -e $LOCK_FILE ]; then
    exit
else
    touch $LOCK_FILE
fi

if [ $ACTUAL_SSID = $HOME_SSID ]; then
    rsync -avze ssh --delete /home/jcmarsh/Dropbox/ jcmarsh@lem4.local:~/liftsphere
fi

rm $LOCK_FILE
