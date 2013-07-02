/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o ft232h_eeprom ft232h_eerom.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
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
	ptdata->Version = 5; //FT232H
	ptdata->VendorId = 0x0403;
	ptdata->ProductId = 0x6014;
	ptdata->Manufacturer = "DLRC";
	ptdata->ManufacturerId = "FT";
	ptdata->Description = "DLRC SNAPNODE Plus Rev 2.1";
	ptdata->SerialNumber = "SNPxxxxx"; // if fixed, or NULL

	ptdata->MaxPower = 500;
	ptdata->PnP = 1;
	ptdata->SelfPowered = 0;
	ptdata->RemoteWakeup = 0;

	//FT232H
	ptdata->PullDownEnableH = 1; // non-zero if pull down enabled
	ptdata->SerNumEnableH = 1; // non-zero if serial number to be used
	ptdata->ACSlowSlewH = 0; // non-zero if AC pins have slow slew
	ptdata->ACSchmittInputH = 0; // non-zero if AC pins are Schmitt input
	ptdata->ACDriveCurrentH = 4; // valid values are 4mA, 8mA, 12mA, 16mA
	ptdata->ADSlowSlewH = 0; // non-zero if AD pins have slow slew
	ptdata->ADSchmittInputH = 0; // non-zero if AD pins are Schmitt input
	ptdata->ADDriveCurrentH = 4; // valid values are 4mA, 8mA, 12mA, 16mA
	ptdata->Cbus0H = 0; // Cbus Mux control
	ptdata->Cbus1H = 0; // Cbus Mux control
	ptdata->Cbus2H = 0; // Cbus Mux control
	ptdata->Cbus3H = 1; // Cbus Mux control
	ptdata->Cbus4H = 2; // Cbus Mux control
	ptdata->Cbus5H = 0; // Cbus Mux control
	ptdata->Cbus6H = 0; // Cbus Mux control
	ptdata->Cbus7H = 0; // Cbus Mux control
	ptdata->Cbus8H = 0; // Cbus Mux control
	ptdata->Cbus9H = 0; // Cbus Mux control
	ptdata->IsFifoH = 0; // non-zero if interface is 245 FIFO
	ptdata->IsFifoTarH = 0; // non-zero if interface is 245 FIFO CPU target
	ptdata->IsFastSerH = 0; // non-zero if interface is Fast serial
	ptdata->IsFT1248H = 0; // non-zero if interface is FT1248
	ptdata->FT1248CpolH = 0; // FT1248 clock polarity - clock idle high (1) or clock idle low (0)
	ptdata->FT1248LsbH = 0; // FT1248 data is LSB (1) or MSB (0)
	ptdata->FT1248FlowControlH = 0; // FT1248 flow control enable
	ptdata->IsVCPH = 1; // non-zero if interface is to use VCP drivers
	ptdata->PowerSaveEnableH = 0; // non-zero if using ACBUS7 to save power for self-powered designs
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
		fprintf(stderr, "FT232H EEPROM programmer\n");
		fprintf(stderr, "CopyRight (C) DLRC <ruishengleen@gmail.com>\n");
		fprintf(stderr, "syntax: %s iPort serialString\n", argv[0]);
		return 1;
	}
	//printf("opening port %d\n", iport);
	FT_SetVIDPID(0x0403, 0x6014);
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
