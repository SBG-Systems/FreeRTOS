#include <stdalign.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

/*
 * FreeRTOS idle hook function.
 */
void vApplicationIdleHook( void );

/*
 * Get memory required for the idle task.
 */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
									StackType_t **ppxIdleTaskStackBuffer,
									uint32_t *pulIdleTaskStackSize );

/*
 * Get memory required for the timer task.
 */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
									 StackType_t **ppxTimerTaskStackBuffer,
									 uint32_t *pulTimerTaskStackSize );

/*
 * Idle task control block.
 */
static StaticTask_t								 xIdleTaskTCB;

/*
 * Idle task stack.
 */
static alignas( portBYTE_ALIGNMENT ) StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

/*
 * Timer task control block.
 */
static StaticTask_t xTimerTaskTCB;

/*
 * Timer task stack.
 */
static alignas( portBYTE_ALIGNMENT ) StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/*
 * Report the number of priorities to OpenOCD.
 */
const int __attribute__((externally_visible)) uxTopUsedPriority = configMAX_PRIORITIES - 1;

void vApplicationIdleHook( void )
{
	_wait_for_interrupt();
}

void vApplicationGetIdleTaskMemory (StaticTask_t **ppxIdleTaskTCBBuffer,
									StackType_t **ppxIdleTaskStackBuffer,
									uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer	= &xIdleTaskTCB;
	*ppxIdleTaskStackBuffer	= uxIdleTaskStack;
	*pulIdleTaskStackSize	= configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
									 StackType_t **ppxTimerTaskStackBuffer,
									 uint32_t *pulTimerTaskStackSize )
{
	*ppxTimerTaskTCBBuffer		= &xTimerTaskTCB;
	*ppxTimerTaskStackBuffer	= uxTimerTaskStack;
	*pulTimerTaskStackSize		= configTIMER_TASK_STACK_DEPTH;
}
