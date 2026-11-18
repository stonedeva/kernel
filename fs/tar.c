// initrd.tar
#include "./tar.h"
#include "./printf.h"
#include "./memory.h"

#define TYPEFLAG_OFFSET 156

struct tar_header {
    uint8_t name[100];
    uint8_t mode[8];
    uint8_t uid[8];
    uint8_t gid[8];
    uint8_t size[12];
    uint8_t mtime[12];
    uint8_t chksum[8];
    uint8_t typeflag;
    uint8_t linkname[100];
    uint8_t magic[6];
    uint8_t version[2];
    uint8_t uname[32];
    uint8_t gname[32];
    uint8_t devmaj[8];
    uint8_t devmin[8];
    uint8_t prefix[155];
    uint8_t padding[12];
} __attribute__((packed));

static unsigned _oct2int(const char* s, int n) 
{
    unsigned v = 0;
    for (int i = 0; i < n && s[i] >= '0' && s[i] <= '7'; i++) {
        v = v * 8 + (s[i] - '0');
    }
    return v;
}

uint8_t* tar_find(uint8_t* tar_start, char* path, uint32_t* len)
{
    uint8_t* p = tar_start;
    while (1) {
	struct tar_header* head = (struct tar_header*)p;
	if (head->name[0] == 0) {
	    return NULL;
	}

	unsigned size = _oct2int((const char*)head->size, 11);
	if ((head->typeflag == 0 || head->typeflag == '0')) {
	    if (strcmp(head->name, path) == 0) {
		*len = size;
		return p + 512;
	    }
	}

	p += 512 + ((size + 511) & ~511u);
    }
}
