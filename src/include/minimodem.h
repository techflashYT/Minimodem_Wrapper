#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define MODE_TRANSMIT true
#define MODE_RECEIVE  false
#define handshakeBaudRate   ((uint16_t)40)
#define handshakeConfidence ((double)2.25)
extern bool minimodem(uint8_t *file, size_t size, bool mode, uint16_t baudRate, double confidence);
typedef struct {
	void *rsPtr;
	uint16_t parityBytes;
} __ecc_t;
// ecc_t is a pointer to the above struct
typedef __ecc_t *ecc_t;
// creates an ecc_t struct with the correct values
ecc_t   eccCreate  (uint8_t parityPercent, size_t dataSize);
/*
	encodes data based on the data pointer and size.
	encodedDataPtr is set the the pointer of the encoded data.
	returns the size of the data + ECC info
*/
size_t  eccEncode  (ecc_t ecc, void *data, size_t dataSize, uint8_t **encodedDataPtr);
/*
	decodes ECC data based on the encoded data pointer and encoded size.
	decodedDataPtr is set the the pointer of the decoded data.
	returns the size of the data
*/
size_t  eccDecode  (ecc_t ecc, uint8_t *encodedData, size_t dataSize, uint8_t **decodedDataPtr);
// cleans up the ecc_t struct.  Do not attempt to use the variable after calling this function.
void    eccDestroy (ecc_t ecc);
typedef struct {
	uint16_t baudRate;
	float    confidence;
	bool     mode;
	char    *fileName;
	char	*additionalArgs;
} options_t;
extern options_t figureOutArgs(int argc, char *argv[]);
extern void handshakeCl();
extern void txCL_HSTART();
extern void rxSV_HSTART();

extern void handshakeSv();
extern void rxCL_HSTART();
extern void txSV_HSTART();
extern void rxCL_TFMMWVer();

extern char      *handshakeStr;
extern char      *resendStr;
extern uint8_t   *readBuf;
extern char      *allGoodStr;
extern options_t  opts;
#define VER_MA (0)
#define VER_MI (1)
#define VER_PA (0)
#define B_YEL  "\x1b[1;33m"
#define B_CYAN "\x1b[1;36m"
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define RESET  "\x1b[0m"
extern uint8_t globalParityOffset;