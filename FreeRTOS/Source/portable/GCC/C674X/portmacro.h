#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "c6x.h"

#define portCHAR					char
#define portFLOAT					float
#define portDOUBLE					double
#define portLONG					long
#define portSHORT					short
#define portSTACK_TYPE				unsigned portLONG
#define portBASE_TYPE				portLONG

typedef long						BaseType_t;
typedef portSTACK_TYPE				StackType_t;
typedef unsigned long				UBaseType_t;

typedef unsigned long				TickType_t;
#define portMAX_DELAY				( ( TickType_t ) 0xfffffffful )

#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			8

static inline void _wait_for_interrupt( void )
{
	__asm volatile ( "idle" ::: "memory" );
}

#define portDISABLE_INTERRUPTS()	_disable_interrupts()
#define portENABLE_INTERRUPTS()		_enable_interrupts()

#define portSET_INTERRUPT_MASK_FROM_ISR()		_disable_interrupts()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	_restore_interrupts(x)

#define portCRITICAL_NESTING_IN_TCB	(1)

void vTaskEnterCritical(void);
void vTaskExitCritical(void);
#define portENTER_CRITICAL()	vTaskEnterCritical()
#define portEXIT_CRITICAL()		vTaskExitCritical()

#define portNOP()

#define portYIELD() __asm volatile ( "swe" ::: "memory" );

#define portYIELD_FROM_ISR(x)	do { if (x) { vTaskSwitchContext(); } } while (0)

#define portTASK_FUNCTION_PROTO(fn, pArg)	void fn( void *pArg )
#define portTASK_FUNCTION(fn, pArg)			void fn( void *pArg )

#define pvPortMalloc	malloc
#define vPortFree		free

#endif /* PORTMACRO_H */
