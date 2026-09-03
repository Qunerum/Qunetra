#ifndef ATA_H
#define ATA_H

#include "../../utility/types.h"

void ata_readSector(const uint64 lba, uint8 *buf);
void ata_writeSector(const uint64 lba, const uint8 *buf);
uint32 ata_getSectorCount();

#endif
