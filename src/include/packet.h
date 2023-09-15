#pragma once
#include <stdint.h>
#include <stddef.h>
#define DUMB_STATIC_ASSERT(test) typedef char assertion_on_mystruct[( !!(test) )*2-1 ]

typedef struct {
	// ASCII {'T', 'F'}
	char magic[2];

	// multiples of 64, based on how much data to send
	// this maxes out at 16320 bytes, which is a ton for one packet.
	uint8_t packetSize;

	// percentage of ECC data
	uint8_t eccPercent;

	// CRC32 Checksum
	uint32_t crc32chksum;

	// actual data + ECC
	uint8_t data[];
} __attribute__ ((packed)) packet_t;

extern void PKT_Assemble(packet_t *pktMem, uint8_t eccPercent, uint8_t *data, size_t dataSize);