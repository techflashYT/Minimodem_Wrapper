#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../libs/reed-solomon-ecc/rs.h"
void eccEncode(void *data, size_t dataSize, uint8_t parityPercent) {
	(void)data;
	(void)dataSize;
	double dataShards   = 10;
	size_t blockSize    = 10;
	double parityShards = ceil(dataShards * ((double)(parityPercent / ((double)(100)))));

	reed_solomon *rs = reed_solomon_new(dataShards, parityShards);

	size_t shards = rs->shards;
	size_t blocks = (dataSize + blockSize - 1) / blockSize;
	blocks = ((blocks + dataShards - 1) / dataShards) * dataShards;
	size_t something = blocks / dataShards;
	size_t fecBlocks = something * parityShards;
	shards = blocks + fecBlocks;
	printf("===rs encoding===\r\ndataShards: %.2f\r\nparityShards: %.2f\r\nsize: %lu\r\nblocks: %lu\r\n=================\r\n", dataShards, parityShards, dataSize, blocks);

	uint8_t **dataBlocks = (uint8_t **)malloc(shards * sizeof(uint8_t **));
	for (size_t i = 0; i < shards; i++) {
		dataBlocks[i] = data + i * blockSize;
    }
	reed_solomon_encode2(rs, dataBlocks, blocks + fecBlocks, blockSize);
	printf("encoded\r\n");
}