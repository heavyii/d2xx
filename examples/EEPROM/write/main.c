/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o write main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

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
		fprintf(stderr, "syntax: %s iPort serialPrefix serialNumber\n", argv[0]);
		return -1;
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
		printf("FT_Open port %d failed\n", iport);
		return 1;
	}

	Data.Signature1 = 0x00000000;
	Data.Signature2 = 0xffffffff;
	Data.Version = 5;	//FT232H
	Data.VendorId = 0x0403;
	Data.ProductId = 0x6014;
	Data.Manufacturer =  "DLRC";
	Data.ManufacturerId = "FT";
	Data.Description = "DLRC SNAPNODE Plus Rev 2.0";
	Data.SerialNumber = serialstr;		// if fixed, or NULL

	Data.MaxPower = 500;
	Data.PnP = 1;
	Data.SelfPowered = 0;
	Data.RemoteWakeup = 1;

	//FT232H
	Data.PullDownEnableH = 1;	// non-zero if pull down enabled
	Data.SerNumEnableH = 1;		// non-zero if serial number to be used
	Data.ACSlowSlewH = 0;		// non-zero if AC pins have slow slew
	Data.ACSchmittInputH = 0;	// non-zero if AC pins are Schmitt input
	Data.ACDriveCurrentH = 4;	// valid values are 4mA, 8mA, 12mA, 16mA
	Data.ADSlowSlewH = 0; 		// non-zero if AD pins have slow slew
	Data.ADSchmittInputH = 0;	// non-zero if AD pins are Schmitt input
	Data.ADDriveCurrentH = 4;	// valid values are 4mA, 8mA, 12mA, 16mA
	Data.Cbus0H = 0;			// Cbus Mux control
	Data.Cbus1H = 0;			// Cbus Mux control
	Data.Cbus2H = 0;			// Cbus Mux control
	Data.Cbus3H = 1;			// Cbus Mux control
	Data.Cbus4H = 2;			// Cbus Mux control
	Data.Cbus5H = 0;			// Cbus Mux control
	Data.Cbus6H = 0;			// Cbus Mux control
	Data.Cbus7H = 0;			// Cbus Mux control
	Data.Cbus8H = 0;			// Cbus Mux control
	Data.Cbus9H = 0;			// Cbus Mux control
	Data.IsFifoH = 0;			// non-zero if interface is 245 FIFO
	Data.IsFifoTarH = 0;		// non-zero if interface is 245 FIFO CPU target
	Data.IsFastSerH = 0;		// non-zero if interface is Fast serial
	Data.IsFT1248H = 0;			// non-zero if interface is FT1248
	Data.FT1248CpolH = 0;		// FT1248 clock polarity - clock idle high (1) or clock idle low (0)
	Data.FT1248LsbH = 0;		// FT1248 data is LSB (1) or MSB (0)
	Data.FT1248FlowControlH = 0;// FT1248 flow control enable
	Data.IsVCPH = 1;			// non-zero if interface is to use VCP drivers
	Data.PowerSaveEnableH = 1;	// non-zero if using ACBUS7 to save power for self-powered designs

#ifdef H2232_DEVICE // If 2232C

	Data.Signature1 = 0x00000000;
	Data.Signature2 = 0xffffffff;
	Data.VendorId = 0x0403;				
	Data.ProductId = 0x6010;
	Data.Manufacturer =  "FTDI";
	Data.ManufacturerId = "FT";
	Data.Description = "SPI";
	Data.SerialNumber = "FT123452";		// if fixed, or NULL
	
	Data.MaxPower = 200;
	Data.PnP = 1;
	Data.SelfPowered = 0;
	Data.RemoteWakeup = 0;
	Data.Rev4 = 0;
	Data.IsoIn = 0;
	Data.IsoOut = 0;
	Data.PullDownEnable = 0;
	Data.SerNumEnable = 0;
	Data.USBVersionEnable = 0;
	Data.USBVersion = 0;

	Data.Rev5 = 1;					// non-zero if Rev5 chip, zero otherwise
	Data.IsoInA = 0;				// non-zero if in endpoint is isochronous
	Data.IsoInB = 0;				// non-zero if in endpoint is isochronous
	Data.IsoOutA = 0;				// non-zero if out endpoint is isochronous
	Data.IsoOutB = 0;				// non-zero if out endpoint is isochronous
	Data.PullDownEnable5 = 0;		// non-zero if pull down enabled
	Data.SerNumEnable5 = 1;			// non-zero if serial number to be used
	Data.USBVersionEnable5 = 0;		// non-zero if chip uses USBVersion
	Data.USBVersion5 = 0x0200;		// BCD (0x0200 => USB2)
	Data.AIsHighCurrent = 0;		// non-zero if interface is high current
	Data.BIsHighCurrent = 0;		// non-zero if interface is high current
	Data.IFAIsFifo = 1;				// non-zero if interface is 245 FIFO
	Data.IFAIsFifoTar = 0;			// non-zero if interface is 245 FIFO CPU target
	Data.IFAIsFastSer = 0;			// non-zero if interface is Fast serial
	Data.AIsVCP = 0;				// non-zero if interface is to use VCP drivers
	Data.IFBIsFifo = 1;				// non-zero if interface is 245 FIFO
	Data.IFBIsFifoTar = 0;			// non-zero if interface is 245 FIFO CPU target
	Data.IFBIsFastSer = 0;			// non-zero if interface is Fast serial
	Data.BIsVCP = 0;				// non-zero if interface is to use VCP drivers

#endif								

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
