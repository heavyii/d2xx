#!/bin/bash
#<ruishengleen@gmai.com>

SERIAL_ID_FILE=serial_id.csv

#
# get serial string from motelist
#
SERIALLIST=`motelist | grep 'DN' | awk '{print $2}'`

#
# loop to read DS2411's ID and save to file ${SERIAL_ID_FILE}
#
for SERIAL in ${SERIALLIST}
do
	sudo ./msp430-bsl-pc -d ${SERIAL} -i ds2411Id.exe -f 2> /dev/null 1> /dev/null
	echo `motelist | grep ${SERIAL}`
	echo `motelist | grep 'DN' | awk '{print $1}'` >> ${SERIAL_ID_FILE}
	
	read USER_INPUT
done








