const unsigned long __stack_chk_guard = 0xdeadbeef;

void __stack_chk_fail( void );

void __stack_chk_fail( void )
{
	for( ;; );
}
