master="master_device"
slave="slave_device"

#get major number by device name

cd ./ksocket
make
insmod ksocket.ko
cd ../master_device
make
insmod master_device.ko
chmod 666 /dev/$master
cd ../slave_device
make
insmod slave_device.ko
chmod 666 /dev/$slave
cd ../user_program
make
chmod 777 master
chmod 777 slave
