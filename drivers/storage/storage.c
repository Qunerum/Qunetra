#include "../../utility/types.h"
#include "../../utility/cpu.h"

static uint32 pciCfgRead(const uint8 bus, const uint8 device, const uint8 function, const uint8 offset) {
	const uint32 address = (uint32)((1 << 31) |
	(bus << 16) |
	((device & 0x1F) << 11) |
	((function & 0x07) << 8) |
	(offset & 0xFC));
	outl(0xCF8, address);
	return inl(0xCFC);
}
char* getStorageType() {
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
					if (subclass == 0x01) return "ATA"; // IDE / ATA
					else if (subclass == 0x06 && prog_if == 0x01) return "AHCI"; // AHCI / SATA
				}
			}
		}
	}
	return "NONE";
}
// 0 - 503 data
// 504 - 511 next
