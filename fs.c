#include <stdint.h>

/*
 * https://wiki.osdev.org/USTAR
*/
enum BlockTypeFlags {
    BLOCK_NORMAL_FILE = 0,
    BLOCK_HARD_LINK = 1,
    BLOCK_SYMBOL_LINK = 2,
    BLOCK_CHAR_DEV = 3,
    BLOCK_DIR = 4,
    BLOCK_NAMED_PIPE = 5
} __attribute__((packed));

struct Block {
    uint8_t file_name[100];
    uint8_t file_mode[8];
    uint8_t owner_userid;
    uint8_t group_userid;
    uint8_t file_sz[12];
    uint8_t last_mod_time[12];
    uint8_t header_checksum_rec[8];
    enum BlockTypeFlags type_flag;
    uint8_t linked_file_name[100];
    uint8_t ustar[6];
    uint8_t ustar_version[2];
    uint8_t owner_username[32];
    uint8_t owner_groupname[32];
    uint8_t dev_maj_num[8];
    uint8_t dev_min_num[8];
    uint8_t file_name_prefix[155];
} __attribute__((packed));
