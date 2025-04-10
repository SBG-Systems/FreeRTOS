#include <stdalign.h>
#include <stdint.h>

#include "boot.h"

alignas(8) uint8_t bootStack[ BOOT_STACK_SIZE ];
