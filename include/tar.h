#ifndef _TAR_H_
#define _TAR_H_

#include <stdint.h>

uint8_t* tar_find(uint8_t* tar_start, char* path, uint32_t* len);

#endif // _TAR_H_
