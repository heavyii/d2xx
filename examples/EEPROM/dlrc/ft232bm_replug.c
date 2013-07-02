/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o ft232bm_replug ft232bm_replug.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

/*
 * replug all ft232h devices
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

int main(int argc, char *argv[])
{
	FT_STATUS ftStatus;
	const int MAX_DEV = 128;
	FT_HANDLE ftHd[MAX_DEV];
	int ftHdNum = 0;
	int iport;
	(void)argc;

	printf("%s - replug all FT232BM devices\n", argv[0]);
	printf("CopyRight (C) DLRC <ruishengleen@gmail.com>\n");

	ftHdNum = 0;
	FT_SetVIDPID(0x0403, 0x6001);
	for (iport = 0; iport < MAX_DEV; iport++) {
		ftStatus = FT_Open(iport, &ftHd[iport]);
		if (ftStatus != FT_OK) {
			break;
		}

		ftHdNum++;
	}

	if (ftHdNum == 0) {
		fprintf(stderr, "no ft232h device found\n");
		return -1;
	}

	// replug and close
	for (iport = 0; iport < ftHdNum; iport++) {
		FT_CyclePort(ftHd[iport]);
		FT_Close(ftHd[iport]);
	}
	return 0;
}
