#include <stddef.h>

#include <FreeRTOS.h>
#include <task.h>

#include "syscall.h"

static __thread volatile int a;
static __thread volatile int b = 123;

static void run ( void *pArg )
{
	( void ) pArg;

	/* The purpose of these lines is to make the compiler generate TLS accesses
	and allow users to check those accesses with objdump. */
	a++;
	b++;
}

int main( void )
{
	/* XXX This example is not entirely functional. It is meant as a
	template for an actual project using an SBG tic6x toolchain along
	with FreeRTOS and TLS support. */

	syscallInit();

	xTaskCreate( run, "run", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate( run, "run", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

	vTaskStartScheduler();

	return 0;
}
