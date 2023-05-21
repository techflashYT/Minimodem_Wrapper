#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minimodem.h>
// these are very repetitive because we need to make sure that, even in a bad connection, at least some of it will come through.
char      *handshakeStr = "TFSftMmdmWrp";
char      *resendStr    = "RESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESENDRESEND";
char      *allGoodStr   = "GOOD";
uint8_t   *readBuf;
options_t  opts;