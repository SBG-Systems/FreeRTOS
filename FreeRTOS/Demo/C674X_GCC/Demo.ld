ENTRY(bootStart)

MEMORY
{
	FMW_RAM		(rwx)	: ORIGIN = 0xc0000000,	LENGTH = 32M
	HEAP_RAM	(rw)	: ORIGIN = 0xc2000000,	LENGTH = 32M
}

PHDRS
{
	text		PT_LOAD		FLAGS(5);
	near		PT_LOAD		FLAGS(4);
	const		PT_LOAD		FLAGS(4);
	far			PT_LOAD		FLAGS(6);
	heap		PT_LOAD		FLAGS(6);
}

SECTIONS
{
	.text :
	{
		*(.text)
		*(.text.*)
	} > FMW_RAM : text

	/*
	 * Neither the DSBT model (-mdsbt) nor near DP-relative addressing (-msdata=)
	 * are used by either the toolchain libraries or the executable. However, EHTYPE
	 * relocations are specified to be DP-relative. The data pointer register is set
	 * to the address of this symbol, which normally refers to the first near data
	 * section, however it could be anything, since data addressing only uses far accesses.
	 */
	PROVIDE_HIDDEN (__c6xabi_DSBT_BASE = .);

	.near :
	{
		*(.rodata)
		*(.rodata.*)
	} > FMW_RAM : near

	.const :
	{
		*(.const)
		*(.const.*)
	} > FMW_RAM : const

	/*
	 * The .c6xabi.extab and .c6xabi.exidx sections are used for stack unwinding
	 * by some of the C/C++ standard library functions.
	 */

	.c6xabi.extab :
	{
		*(.c6xabi.extab*)
	} > FMW_RAM : const

	.c6xabi.exidx :
	{
		PROVIDE_HIDDEN(__exidx_start = .);
		*(.c6xabi.exidx*)
		PROVIDE_HIDDEN(__exidx_end = .);
	} > FMW_RAM : const

	/*
	 * The .preinit_array and .init_array sections are used by __libc_init_array
	 * to call all the constructors of global objects.
	 */

	.preinit_array :
	{
		PROVIDE_HIDDEN(__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN(__preinit_array_end = .);
	} > FMW_RAM : const

	.init_array :
	{
		PROVIDE_HIDDEN(__init_array_start = .);
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
		KEEP(*(.init_array .ctors))
		PROVIDE_HIDDEN (__init_array_end = .);
	} > FMW_RAM : const

	.tls_tmpl :
	{
		__tls_tmpl_start = .;
		*(.tls_tmpl)
		__tls_tmpl_end = .;
	} > FMW_RAM : const

	/*
	 * Note that the toolchain used doesn't implement the C6000 EABI regarding
	 * thread-local storage. Instead, the gcc TLS emulation layer has been
	 * overriden to provide a custom ABI for the tic6x architecture.
	 */

	.tls_var :
	{
		*(.tls_var)
	} > FMW_RAM : const

	.fardata :
	{
		*(.fardata)
		*(.fardata.*)
	} > FMW_RAM : far

	/* NOLOAD sections */

	.far (NOLOAD) :
	{
		ld_bss_start = .;
		*(.far)
		*(.far.*)
		ld_bss_end = .;
	} > FMW_RAM : far

	.heap (NOLOAD) :
	{
		*(.heap)
	} > HEAP_RAM : heap
}
