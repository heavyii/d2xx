#!/bin/sh
# rewrite ft232h's eeprom keeping its usb string serial

while [ 1 ]
do
	sudo rmmod ftdi_sio.ko 2> /dev/null
	SERIAL=`./read | grep SerialNumber | awk '{print $3}'`
	if [ "$SERIAL" = "" ] 
	then
		echo "empty"
	else
		./ft232h_eeprom 0 $SERIAL
		echo "done: `./read | grep SerialNumber | awk '{print $3}'`"
	fi
	read INPUT
done
