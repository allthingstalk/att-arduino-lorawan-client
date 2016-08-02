#include <LoRaModem.h>

int LoRaModem::maxPayloadForSF(short spreading_factor) { {
	if (spreading_factor < 0) {
		spreading_factor = getParam(SP_FACTOR) + 6;
	}
	if (spreading_factor >= 10) {
		return 51;
	} else if (spreading_factor < 9) {
		return 222;
	return 115;
}