#include "boot.h"

.func bootStart
.global bootStart
.p2align 10
bootStart:
		mvkl	.s2		__c6xabi_DSBT_BASE, b14
		mvkh	.s2		__c6xabi_DSBT_BASE, b14
		mvkl	.s2		bootStack, b15
		mvkh	.s2		bootStack, b15
		mvkl	.s2		( BOOT_STACK_SIZE - 8 ), b0
		mvkh	.s2		( BOOT_STACK_SIZE - 8 ), b0
		add		.d2		b15, b0, b15

		mvk		.s2		0, b4
		mvc		.s2		b4, fadcr
		mvc		.s2		b4, fmcr

		callp	.s2		bootClearBss, b3
		callp	.s2		main, b3
.endfunc

//
// Clear the .bss section.
//
// Implemented in assembly to guarantee that the stack, which is normally
// in the bss section, may not be used.
//
.func bootClearBss
bootClearBss:
		mvk		.s2		0, b0
		mvkl	.s1		ld_bss_start, a0
		mvkh	.s1		ld_bss_start, a0
		mvkl	.s1		ld_bss_end, a1
		mvkh	.s1		ld_bss_end, a1

1:
		cmpeq	.l1		a0, a1, a2
[a2]	bnop	.s2		b3, 5
		stb		.d1t2	b0, *a0++
		bnop	.s2		1b, 5
.endfunc
