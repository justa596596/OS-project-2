#!/bin/sh
module="driver"
device="mydriver"

#remove module
/sbin/rmmod ./$module.ko $* 

#remove inode
rm /dev/$device 




