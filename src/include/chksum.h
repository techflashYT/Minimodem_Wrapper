#include <stdint.h>
#include <stddef.h>
#define POLYNOMIAL 0xEDB88320L
#define BUFFER_SIZE 4096
static uint32_t CHK_GenCRC32(uint8_t *data, size_t dataSize) {
	uint32_t crc = UINT32_MAX;
    for (size_t i = 0; i < dataSize; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ ((crc & 1) ? POLYNOMIAL : 0);
        }
    }
    return crc ^ UINT32_MAX;

}

// avoid poluting the rest of the codebase with these values
#undef POLYNOMIAL
#undef BUFFER_SIZE