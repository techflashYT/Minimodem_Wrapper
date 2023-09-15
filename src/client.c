#include <opts.h>
#include <log.h>
#include <minimodem.h>
#include <packet.h>
#include <version.h>

static void CL_Handshake();
void CL_Start() {
	CL_Handshake();
}

static void CL_Handshake() {
	// CL_HELLO
	// - Say hi to the server, see if there's anybody listening on the other end.
	packet_t *pkt = malloc(64);

	// hello info
	uint8_t data[] = {
		// magic bytes 'TFMW'
		'T', 'F', 'M', 'W',
		
		// version numbers in binary
		VER_Major, VER_Minor, VER_Patch,
		
		// 'C' for client mode
		'C'
	};
	PKT_Assemble(pkt, 25, data, sizeof(data));
	debug("made hello packet");

	free(pkt);
}
