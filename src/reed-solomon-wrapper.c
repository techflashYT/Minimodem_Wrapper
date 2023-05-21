#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <minimodem.h>
#include <fec.h>
ecc_t eccCreate(uint8_t parityPercent, size_t dataSize) {
	ecc_t ecc = malloc(sizeof(ecc_t));
	// calculate some stuff
	ecc->parityBytes = ceil(dataSize * ((double)(parityPercent / ((double)(100)))));

	// create the rs structure
	ecc->rsPtr =  init_rs_char(8, 0x187, 112, 11, ecc->parityBytes, 0);
	return ecc;
}
size_t eccEncode(ecc_t rs, void *data, size_t dataSize, uint8_t **encodedDataPtr) {
	printf("Encoding %s%lu%s bytes at address %s%p%s\r\n", B_CYAN, dataSize, RESET, B_YEL, data, RESET);
	// allocate some memory
	uint8_t *encoded = malloc(16384);

	// encode
	memcpy(encoded, data, dataSize);
	encode_rs_char(rs, data, encoded + dataSize);
	
	// free leftover memory from the space for the encoded data
	// printf("freeing %s%lu%s bytes from encoded rs data!\r\n", B_CYAN, 16384 - size, RESET);
	// encoded = realloc(encoded, size);

	// give the pointer to the encoded data, as well as the size to the caller
	*encodedDataPtr = encoded;
	printf("Encoded payload of size %s%lu%s bytes at address %s%p%s\r\n", B_CYAN, dataSize + rs->parityBytes, RESET, B_YEL, encoded, RESET);
	return dataSize + rs->parityBytes;
}
void eccDestroy(ecc_t rs) {
	// get rid of the structure
	free_rs_char(rs);
}
size_t eccDecode(ecc_t rs, uint8_t *encodedData, size_t dataSize, uint8_t **decodedDataPtr) {
	// allocate some memory
	uint8_t *decoded = malloc(16384);

	printf("Decoding payload of size %s%lu%s bytes at address %s%p%s into address %s%p%s\r\n", B_CYAN, dataSize, RESET, B_YEL, encodedData, RESET, B_YEL, decoded, RESET);

	
	// decode the data
	size_t size = decode_rs_char(rs, encodedData, NULL, 0);
	// printf("dataSize: %lu\r\nrs->blockLength: %lu\r\n", dataSize, block_length);

	// if it failed, it's -1, print an error and exit
	/*
	if (size == -1 || size == -2) {
		fprintf(stderr, 
			"%sFailed to perform error correction on data!%s  One of the following might help:\r\n"
			"	- %sDecrease the baud rate%s\r\n"
			"	- %sFiddle with confidence value%s\r\n"
			"	- %sIncrease parity data percent %s(WILL make transfer take longer)%s\r\n"
			"If you feel that all of the values are good and yet it's still getting corrupted, please check your setup, and submit a bug report at the below address:\r\n"
			"%shttps://github.com/techflashYT/Minimodem_Wrapper/issues%s\r\n",
			RED, RESET,
			B_CYAN, RESET,
			B_CYAN, RESET,
			B_CYAN, B_YEL, RESET,
			B_CYAN "\x1b[4m", RESET
		);
		exit(1);
	}
	*/
	// free unused mem
	// printf("freeing %s%lu%s bytes from decoded rs data!\r\n", B_CYAN, 16384 - size, RESET);
	// decoded = realloc(decoded, size);

	// set the pointer
	*decodedDataPtr = decoded;
	return size;
}
