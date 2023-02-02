#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minimodem.h>
// these are very repetitive because we need to make sure that, even in a bad connection, at least some of it will come through.
char      *handshakeStr      = "TcflshSftwrMmodemWrpr";
char      *resendStr         = "RESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESEND";
char      *allGoodStr        = "ALLGOODALLGOODALLGOODALLGOODALLGOODALLGOODALLGOODALLGOODALLGOODALLGOODALLGOODALLGOOD";
uint8_t   *handshakeFileName;
uint8_t   *resendFileName;
FILE      *handshakeFile;
FILE      *resendFile;
uint8_t   *readBuf;
options_t  opts;

void initGlobals() {
	handshakeFileName = malloc(34);
	resendFileName    = malloc(34);
	readBuf           = malloc(256);
	if (handshakeFileName == NULL || resendFileName == NULL || readBuf == NULL) {
		fprintf(stderr, "%smalloc() call failed!%s.  Are you out of memory?\r\n", RED, RESET);
		exit(1);
	}
	strcpy(handshakeFileName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	strcpy(resendFileName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	handshakeFile     = fdopen(mkstemp(handshakeFileName), "w+");
	resendFile        = fdopen(mkstemp(resendFileName), "w+");
	
	fwrite(handshakeStr, strlen(handshakeStr), 1, handshakeFile);
	fwrite(resendStr, strlen(resendStr), 1, resendFile);
	fclose(handshakeFile);
	fclose(resendFile);
}