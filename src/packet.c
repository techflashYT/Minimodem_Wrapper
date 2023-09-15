#include <packet.h>
#include <log.h>
#include <chksum.h>
#include <string.h>
#include <assert.h>

void PKT_Assemble(packet_t *pktMem, uint8_t eccPercent, uint8_t *data, size_t dataSize) {
	#ifdef ALLOW_DEBUG
	// it should never point to the zero page
	// assert(pktMem < (packet_t *)0x1000);

	// data pointer should also not point to the zero page
	// assert(data < (uint8_t *)0x1000);

	// we shouldn't be sending 0 bytes...
	assert(dataSize != 0);
	#endif
	(void)eccPercent;


	pktMem->magic[0] = 'T';
	pktMem->magic[1] = 'F';

	// pktMem->eccPercent = 20;
	pktMem->packetSize = dataSize;
	
	memcpy(pktMem->data, data, dataSize);

	pktMem->crc32chksum = 0;
	pktMem->crc32chksum = CHK_GenCRC32((uint8_t *)pktMem, dataSize);

	debug("Packet created");
}