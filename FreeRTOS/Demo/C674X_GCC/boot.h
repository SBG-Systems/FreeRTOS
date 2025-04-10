#ifndef BOOT_H
#define BOOT_H

#ifndef __ASSEMBLER__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#endif /* __ASSEMBLER__ */

/*
 * Boot stack size, in bytes.
 */
#define BOOT_STACK_SIZE 4096

#ifndef __ASSEMBLER__

/*!
 * Boot stack.
 */
extern uint8_t bootStack[ BOOT_STACK_SIZE ];

#ifdef __cplusplus
}
#endif

#endif // __ASSEMBLER__

#endif /* BOOT_H */
