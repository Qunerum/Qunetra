#include "../../utility/memory.h"
#include "../../utility/types.h"
#include "../../utility/cpu.h"
#include "ata.h"

static uint32 pciCfgRead(const uint8 bus, const uint8 device, const uint8 function, const uint8 offset) {
	const uint32 address = (uint32)((1 << 31) |
	(bus << 16) |
	((device & 0x1F) << 11) |
	((function & 0x07) << 8) |
	(offset & 0xFC));
	outl(0xCF8, address);
	return inl(0xCFC);
}
static char* getStorageType() {
	for (uint8 bus = 0; bus < 8; bus++) {
		for (uint8 device = 0; device < 32; device++) {
			for (uint8 function = 0; function < 8; function++) {
				const uint32 vendor_device = pciCfgRead(bus, device, function, 0x00);
				if ((vendor_device & 0xFFFF) == 0xFFFF) continue;
				const uint32 class_reg = pciCfgRead(bus, device, function, 0x08);
				const uint8 class_code = (class_reg >> 24) & 0xFF,
					subclass   = (class_reg >> 16) & 0xFF,
					prog_if    = (class_reg >> 8) & 0xFF;
				if (class_code == 0x01) {
					if (subclass == 0x01) return " IDE / ATA ";
					else if (subclass == 0x06 && prog_if == 0x01) return " AHCI / SATA ";
				}
			}
		}
	}
	return " NONE ";
}

static uint8 *tmpData, *tmpMeta;
static state sectorDirty = false;
static uint64 actualSector = 0;
static void (*sswrite)(const uint64 sector, const uint8 *buf), (*ssread)(const uint64 sector, uint8 *buf);

char* initStorage() {
	tmpData = kmalloc(512);
	sswrite = ata_writeSector;
	ssread = ata_readSector;
	return getStorageType();
}
void closeStorage() { kfree(tmpData); }
static state get_bit(const uint8 val, const uint8 bit_index) {
	if (bit_index > 7) return 0;
	return (val >> bit_index) & 1;
}
static void pushValue(const uint64 value, const uint8 size, const uint64 sector, uint16 *byte) {
	if (size == 0 || byte == 0) return;
	if (size > 8) return;
	if (actualSector != sector) {
		if (sectorDirty) {
			sswrite(actualSector, tmpData);
			sectorDirty = false;
		}
		actualSector = sector;
		ssread(actualSector, tmpData);
	}
	if (*byte >= 512) *byte = 0;
	for (uint8 i = 0; i < size; i++) {
		if (*byte + i >= 512) *byte = 0;
		tmpData[(*byte)++] = (uint8)(value >> (i * 8));
	}
	sectorDirty = true;
}
// = = = = = META
// 0 - 7 last sector: 8 bytes
// 8 - 15 sector with id's: 8 bytes
// 16 - 23 sector with free sectors: 8 bytes
// 24+ other data
static void writeNewID() {

}

// = = = = = DATA
// 0 - 502 data: 503 bytes
// 503 - 511 data: 9 bytes
// A BCDEFGHI
// A - 76543210
// 0 - is deleted | 1 - is start | 2 -  | 3 -  | 4 -  | 5 -  | 6 -  | 7 -

void writeData(uint64 id, uint8* data, uint64* len) {
	if (id == 0 || data == 0 || len == 0) return;
	//
}
