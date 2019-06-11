#!/bin/sh
device="master_device"

#install module
/sbin/insmod ./$device.ko $*

#get major number by device name
major=$(awk "\$2==\"$device\" {print \$1}"  /proc/devices)

#make inode
mknod /dev/$device c $major 0
chmod 666 /dev/$device

