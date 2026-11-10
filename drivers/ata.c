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

#define ATA_CMD_READ 0x20
#define ATA_CMD_WRITE 0x30

static inline void ata_wait_ready()
{
    while (inb(ATA_STATUS) & 0x80);
}

void ata_read_sector(int lba, void* buffer)
{
    uint16_t* buf16 = (uint16_t*)buffer;

    ata_wait_ready();
    outb(ATA_DRIVE_REG, 0xE0 | ((lba << 24) & 0x0F));

    // Read sector
    outb(ATA_SECTOR_COUNT_REG, 1);
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND, ATA_CMD_READ);

    uint8_t status;
    do {
	status = inb(ATA_STATUS);
    } while ((status & 0x80) || !(status & 0x08));

    for (int i = 0; i < 256; i++) {
	buf16[i] = inw(ATA_DATA_PORT);
    }
}

void ata_write_sector(int lba, void* buffer)
{
    const uint16_t* buf16 = (uint16_t*)buffer;

    ata_wait_ready();
    outb(ATA_DRIVE_REG, 0xE0 | ((lba << 24) & 0x0F));

    outb(ATA_SECTOR_COUNT_REG, 1);
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND, ATA_CMD_WRITE);

    // Wait for DRQ
    while (1) {
	uint8_t status = inb(ATA_COMMAND);
	if (status & (0x01 | 0x20))
	    return;
	if (!(status & 0x80) && (status & 0x08))
	    break;
    }

    for (int i = 0; i < 256; i++) {
	outw(ATA_DATA_PORT, buf16[i]);
    }

    while (1) {
        uint8_t status = inb(ATA_COMMAND);

        if (status & (0x01 | 0x20))
            return;
        if (!(status & 0x80) && !(status & 0x08))
            break;
    }
}
