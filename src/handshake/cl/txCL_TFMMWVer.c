#include <stdio.h>
#include <string.h>
#include <minimodem.h>
void txCL_TFMMWVer() {
	printf("Sending CL_TFMMWVer...\r\n");
	char verStr[] = {
		'v',
		VER_MA + '0',
		'.',
		VER_MI + '0',
		'.',
		VER_PA + '0',
		'\0'
	};
	minimodem(verStr, 7, MODE_TRANSMIT, opts.baudRate, 0);
	printf("Sent CL_TFMMWVer!\r\n");
	return;
}