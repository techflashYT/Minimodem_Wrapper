#pragma once
#include <stdint.h>
#include <stddef.h>
#define DUMB_STATIC_ASSERT(test) typedef char assertion_on_mystruct[( !!(test) )*2-1 ]

typedef struct {
	// ASCII {'T', 'F'}
	char magic[2];

	// this maxes out at 255 bytes, which is a ton for one packet.
	uint8_t packetSize;

	// bitfield of status bits
	uint8_t statusOK     : 1;
	uint8_t statusErr    : 1;
	uint8_t statusResent : 1;
	uint8_t statusRsrvd1 : 1;
	uint8_t statusRsrvd2 : 1;
	uint8_t statusRsrvd3 : 1;
	uint8_t statusRsrvd4 : 1;
	uint8_t statusRsrvd5 : 1;

	// CRC32 Checksum
	uint32_t crc32chksum;

	// actual data + ECC
	uint8_t data[];
} __attribute__ ((packed)) packet_t;

extern void PKT_Assemble(packet_t *pktMem, uint8_t eccPercent, uint8_t *data, size_t dataSize);