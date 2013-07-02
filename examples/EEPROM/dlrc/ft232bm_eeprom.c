/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o ft232bm_eeprom ft232bm_eeprom.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

/**
 * build_eeprom_data - build eeprom data structure for FT232H
 */
void build_eeprom_data(FT_PROGRAM_DATA *ptdata) {
	memset(ptdata, 0, sizeof(FT_PROGRAM_DATA));

	ptdata->Signature1 = 0x00000000;
	ptdata->Signature2 = 0xffffffff;
	ptdata->Version = 0; //original
	ptdata->VendorId = 0x0403;
	ptdata->ProductId = 0x6001;
	ptdata->Manufacturer = "HUAWEI";
	ptdata->ManufacturerId = "FT";
	ptdata->Description = "HUAWEI Node Rev.B";
	ptdata->SerialNumber = "DNxxxxxx"; // if fixed, or NULL

	ptdata->MaxPower = 400;
	ptdata->PnP = 1;
	ptdata->SelfPowered = 0;
	ptdata->RemoteWakeup = 1;

	ptdata->Rev4 = 0x1;					// non-zero if Rev4 chip, zero otherwise
	ptdata->IsoIn = 0;				// non-zero if in endpoint is isochronous
	ptdata->IsoOut = 0;				// non-zero if out endpoint is isochronous
	ptdata->PullDownEnable = 0;		// non-zero if pull down enabled
	ptdata->SerNumEnable = 1;			// non-zero if serial number to be used
	ptdata->USBVersionEnable = 1;		// non-zero if chip uses USBVersion
	ptdata->USBVersion = 0x0200;			// BCD (0x0200 => USB2)
}

int main(int argc, char *argv[])
{
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	int iport;
	FT_PROGRAM_DATA Data;
	char *serialstr;
	if(argc == 3) {
		sscanf(argv[1], "%d", &iport);
		serialstr = argv[2];
	}
	else {
		fprintf(stderr, "FT232BM EEPROM programmer\n");
		fprintf(stderr, "CopyRight (C) DLRC <ruishengleen@gmail.com>\n");
		fprintf(stderr, "syntax: %s iPort serialString\n", argv[0]);
		return 1;
	}
	//printf("opening port %d\n", iport);
	FT_SetVIDPID(0x0403, 0x6001);
	ftStatus = FT_Open(iport, &ftHandle0);
	if(ftStatus != FT_OK) {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open port (%d) failed\n", iport);
		printf("return 1\n");
		return 1;
	}

	ftStatus = FT_EraseEE(ftHandle0);
	if(ftStatus != FT_OK) {
		printf("Erase failed\n");
	}

	build_eeprom_data(&Data);
	Data.SerialNumber = serialstr;

	ftStatus = FT_EE_Program(ftHandle0, &Data);
	if(ftStatus != FT_OK) {
		printf("FT_EE_Program failed (%d)\n", (int)ftStatus);
		FT_Close(ftHandle0);
		return -1;
	}
	//FT_CyclePort(ftHandle0);
	FT_Close(ftHandle0);
	return 0;
}
