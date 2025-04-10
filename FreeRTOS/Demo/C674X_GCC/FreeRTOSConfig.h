#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <assert.h>

#define configSUPPORT_STATIC_ALLOCATION						1
#define configSUPPORT_DYNAMIC_ALLOCATION					1
#define configUSE_PREEMPTION								1
#define configUSE_IDLE_HOOK									1
#define configUSE_TICK_HOOK									0
#define configCPU_CLOCK_HZ									300000000
#define configTICK_RATE_HZ									1000
#define configMAX_PRIORITIES								2
#define configMINIMAL_STACK_SIZE							256
#define configTOTAL_HEAP_SIZE								(32 * 1024 * 1024)
#define configMAX_TASK_NAME_LEN								28
#define configUSE_16_BIT_TICKS								0
#define configIDLE_SHOULD_YIELD								0
#define configUSE_COUNTING_SEMAPHORES						1
#define configUSE_MUTEXES									1
#define configUSE_RECURSIVE_MUTEXES							1
#define configUSE_QUEUE_SETS								0

#define configUSE_TIMERS									1
#define configTIMER_SERVICE_TASK_NAME						"rtosTimer"
#define configTIMER_TASK_PRIORITY							1
#define configTIMER_QUEUE_LENGTH							64
#define configTIMER_TASK_STACK_DEPTH						configMINIMAL_STACK_SIZE

#define configASSERT										assert

#ifdef SBG_ENABLE_TRACE
#define configCHECK_FOR_STACK_OVERFLOW						2
#define configUSE_TRACE_FACILITY							1
#else // SBG_ENABLE_TRACE
#define configCHECK_FOR_STACK_OVERFLOW						0
#define configUSE_TRACE_FACILITY							0
#endif // SBG_ENABLE_TRACE

#define configUSE_CO_ROUTINES								0
#define configMAX_CO_ROUTINE_PRIORITIES						2

#define INCLUDE_vTaskPrioritySet							0
#define INCLUDE_uxTaskPriorityGet							0
#define INCLUDE_vTaskDelete									1
#define INCLUDE_vTaskCleanUpResources						0
#define INCLUDE_vTaskSuspend								1
#define INCLUDE_vTaskDelayUntil								1
#define INCLUDE_vTaskDelay									1
#define INCLUDE_xTaskGetSchedulerState						1
#define INCLUDE_xTaskGetCurrentTaskHandle					1
#define INCLUDE_xTaskGetIdleTaskHandle						1

/*
 * Newlib support.
 *
 * See http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 */
#define configUSE_NEWLIB_REENTRANT							1

/*
 * Size of the thread-local storage data block, in bytes.
 *
 * Thread-local storage is normally only used for C++ exception handling
 * thread safety. Since all tasks have a TLS block, this value should
 * remain small.
 */
#define configTLS_DATA_SIZE									32

#endif /* FREERTOS_CONFIG_H */
