#ifndef _ATA_H_
#define _ATA_H_

void ata_read_sector(int lba, void* buffer);
void ata_write_sector(int lba, void* buffer);

#endif // _ATA_H_
