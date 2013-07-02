#!/bin/bash
#<ruishengleen@gmai.com>

while [ 1 ]
do	
	sh ./writeft232bm.sh
	echo '#' `./motelist | grep 'DN'`
	read INPUT
done


exit 0

#sh ./erase.sh
#sh ./program.sh








