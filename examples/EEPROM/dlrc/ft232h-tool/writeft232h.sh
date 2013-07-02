#!/bin/bash
#
# read the starting serial number form file ${SERIALNUMCFG}, write all ft232h devices' eeprom 
# and save written serial string to ${SERIALLOGFILE}, update serial number to ${SERIALNUMCFG}.
# progam will replug all ft232h devices to let kernel reread usb infomation.
# after this, we perform jtag test here to read DS2411's ID, then save usb serial sting and 
# DS2411's ID to gather to file.

#
#configure
#
SERIALLOGFILE=serial_used.log
SERIALNUMCFG=serialnumber.cfg
SERIAL_ID_FILE=serial_id_log.txt
SERIAL_PREFIX='SNP'
SERIAL_NUMBER=`cat ${SERIALNUMCFG}`

#save current serial number
SERIAL_NUMBER_START=${SERIAL_NUMBER}

#must unload sio driver
sudo rmmod ftdi_sio 2> /dev/null

#make serial string in the formate of SNPxxxxx
SERIAL=${SERIAL_PREFIX}`printf "%.5d" ${SERIAL_NUMBER}`

#write eeprom
sudo ./ft232h_eeprom `printf %0d $DEVNUM`  ${SERIAL} > /dev/null
if [ $? = "0" ]
then
	#write serial string to log file
	echo ${SERIAL} >> ${SERIALLOGFILE}
		#serial number increase
	SERIAL_NUMBER=$((SERIAL_NUMBER+1))
else
	echo '#USB ERROR: failed to write eeprom  '${SERIAL}
	exit 1
fi

#
# replug all ft232h devices
#
sudo ./ft232h_replug > /dev/null

DSID=
COUNTER=0
while [ "${DSID}" = "" -a  $COUNTER -lt 10 ]
do
	DSID=`sudo ./mspdebug -s ${SERIAL} -j ft232h "erase" "reset" \
	"load ds2411Id.exe" "reset" "setbreak 0x4564" "run" "md 0x1000 8" 2>/dev/null | \
	grep '01000: 7e' | awk   -F 7e '{print $2}'`
	COUNTER=$((COUNTER+1))
done
if [ "${DSID}" = "" ]
then
	echo '#JTAG ERROR: USB to JTAT error '${SERIAL}
	exit 1
else
	echo "serial:" ${SERIAL} ', ds2411(' ${DSID} ')'
	echo "${SERIAL},${DSID}" >> ${SERIAL_ID_FILE}
fi

#save new serial number to config file
echo ${SERIAL_NUMBER} > ${SERIALNUMCFG}





