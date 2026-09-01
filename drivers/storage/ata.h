#ifndef ATA_H
#define ATA_H

#include "../../utility/types.h"

void ata_readSector(const uint32 lba, uint8 *buf);
void ata_writeSector(const uint32 lba, const uint8 *buf);

#endif
