#!/bin/bash

sudo sh /etc/qemu-ifup tap0
sudo qemu-system-i386 -m 512 -hda ubuntu-test -net nic -net tap,ifname=tap0,script=no,downscript=no
sudo sh /etc/qemu-ifdown tap0
