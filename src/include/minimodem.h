#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define MODE_TRANSMIT true
#define MODE_RECEIVE  false
bool minimodem(uint8_t *data, size_t size, bool mode, uint16_t baudRate, double confidence);
typedef struct {
	uint16_t baudRate;
	float    confidence;
	bool     mode;
	char    *fileName;
} options_t;
options_t figureOutArgs(int argc, char *argv[]);
#define VER_MA (0)
#define VER_MI (1)
#define VER_PA (0)