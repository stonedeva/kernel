#include "./ata.h"
#include "./io.h"
#include <stdint.h>


#define ATA_DRIVE_READY 0x1f6

#define ATA_DATA_PORT 0x1f0
#define ATA_SECTOR_COUNT_REG 0x1f2
#define ATA_LBA_LOW 0x1f3
#define ATA_LBA_MID 0x1f4
#define ATA_LBA_HIGH 0x1f5
#define ATA_DRIVE_REG 0x1f6

/*
 * ATA_STATUS = read
 * ATA_COMMAND = write
*/
#define ATA_STATUS 0x1f7
#define ATA_COMMAND 0x1f7
#define ATA_ALTERN_STATUS_REG 0x3f6

static inline void ata_wait_ready()
{
    while (inb(ATA_STATUS) & 0x80);
}

void ata_read_sector0(void* buffer)
{
    uint16_t* buf16 = (uint16_t*)buffer;

    outb(ATA_DRIVE_REG, 0xE0);

    ata_wait_ready();

    // Read one sector
    outb(ATA_SECTOR_COUNT_REG, 1);
    outb(ATA_LBA_LOW, 0);
    outb(ATA_LBA_MID, 0);
    outb(ATA_LBA_HIGH, 0);
    outb(ATA_COMMAND, 0x20);

    uint8_t status;
    do {
	status = inb(ATA_STATUS);
    } while ((status & 0x80) || !(status & 0x08));

    for (int i = 0; i < 256; i++) {
	buf16[i] = inw(ATA_DATA_PORT);
    }
}
