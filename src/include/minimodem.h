#include <opts.h>
#include <packet.h>

static void minimodem_RX(packet_t *packetPtr) {
	(void)packetPtr;
}

static void minimodem_TX(packet_t *packet) {
	(void)packet;

	packet_t recvPkt;
	minimodem_RX(&recvPkt);
}