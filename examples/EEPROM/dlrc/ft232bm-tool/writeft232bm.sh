#!/bin/bash
#
# read the starting serial number form file ${SERIALNUMCFG}, write all ft232bm devices' eeprom 
# and save written serial string to ${SERIALLOGFILE}, update serial number to ${SERIALNUMCFG}.
# progam will replug all ft232h devices to let kernel reread usb infomation.
# after this, we perform jtag test here to read DS2411's ID, then save usb serial sting and 
# DS2411's ID to gather to file.

#
#configure
#
SERIALLOGFILE=serial_used.log
SERIALNUMCFG=serialnumber.cfg
SERIAL_PREFIX='DN'
SERIAL_NUMBER=`cat ${SERIALNUMCFG}`


#save current serial number
SERIAL_NUMBER_START=${SERIAL_NUMBER}

#must unload sio driver
sudo rmmod ftdi_sio 2> /dev/null

#
#loop to write eeprom of FT232H
#
NODEV=0
while [ ${NODEV} -eq '0' ]
do
	#make serial string in the formate of SNPxxxxx
	SERIAL=${SERIAL_PREFIX}`printf "%.6d" ${SERIAL_NUMBER}`

	#write eeprom
	sudo ./ft232bm_eeprom `printf %0d $DEVNUM`  ${SERIAL} > /dev/null
	if [ $? = "0" ]
	then
		#write serial string to log file
		echo ${SERIAL} >> ${SERIALLOGFILE}

		#serial number increase
		SERIAL_NUMBER=$((SERIAL_NUMBER+1))
	else
		#write eeprom failed, for 2 times, stop it
		NODEV=$((NODEV+1))
	fi

	#next device
	DEVNUM=$((DEVNUM+1))
done

#save new serial number to config file
echo ${SERIAL_NUMBER} > ${SERIALNUMCFG}

#
# replug all ft232h devices
#
sudo ./ft232bm_replug > /dev/null


