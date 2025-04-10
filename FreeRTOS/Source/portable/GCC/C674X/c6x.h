#ifndef C6X_H
#define C6X_H

/* This file provides a few built-ins normally available with the C6000 CGT. */

#define c6x_get_ctrlreg(reg)												\
({																			\
	unsigned int oldval___;													\
																			\
	__asm volatile ("mvc .s2 " reg ", %0" : "=b" (oldval___) : : "memory");	\
																			\
	oldval___;																\
})

#define c6x_set_ctrlreg(reg, value)											\
do																			\
{																			\
	unsigned int newval___ = (unsigned int)(value);							\
																			\
	__asm volatile ("mvc .s2 %0, " reg : : "b" (newval___) : "memory");		\
} while (0)

static inline __attribute__((always_inline)) void _restore_interrupts(unsigned int csr)
{
	c6x_set_ctrlreg("csr", csr);
}

static inline __attribute__((always_inline)) unsigned int _disable_interrupts(void)
{
	unsigned int						 csr;

	csr = c6x_get_ctrlreg("csr");
	csr &= ~(unsigned int)1;
	_restore_interrupts(csr);

	return csr;
}

static inline __attribute__((always_inline)) unsigned int _enable_interrupts(void)
{
	unsigned int						 csr;

	csr = c6x_get_ctrlreg("csr");
	csr |= 1;
	_restore_interrupts(csr);

	return csr;
}

#endif /* C6X_H */
