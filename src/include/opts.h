#include <stdint.h>
typedef struct {
	enum {
		MODE_RX = 0,
		MODE_TX = 1,
		MODE_UNSET = 0xFF
	} mode;

	uint32_t baudRate;
	
} opts_t;
extern opts_t opts;