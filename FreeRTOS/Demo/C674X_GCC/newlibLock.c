#include <stdlib.h>
#include <sys/lock.h>
#include <sys/reent.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "newlibLock.h"

struct __lock
{
	SemaphoreHandle_t mutex;
	StaticSemaphore_t mutexStorage;
};

struct __lock __lock___arc4random_mutex;
struct __lock __lock___atexit_recursive_mutex;
struct __lock __lock___at_quick_exit_mutex;
struct __lock __lock___dd_hash_mutex;
struct __lock __lock___env_recursive_mutex;
struct __lock __lock___malloc_recursive_mutex;
struct __lock __lock___sfp_recursive_mutex;
struct __lock __lock___sinit_recursive_mutex;
struct __lock __lock___tz_mutex;

void __retarget_lock_init( _LOCK_T *ppLock )
{
	struct __lock *pLock;

	assert( ppLock );

	pLock = malloc( sizeof( *pLock ) );

	if( pLock )
	{
		pLock->mutex = xSemaphoreCreateMutexStatic( &pLock->mutexStorage );
	}

	*ppLock = pLock;
}

void __retarget_lock_init_recursive( _LOCK_T *ppLock )
{
	struct __lock *pLock;

	assert( ppLock );

	pLock = malloc( sizeof( *pLock ) );

	if( pLock )
	{
		pLock->mutex = xSemaphoreCreateRecursiveMutexStatic( &pLock->mutexStorage );
	}

	*ppLock = pLock;
}

void __retarget_lock_close( _LOCK_T pLock )
{
	assert( pLock );

	vSemaphoreDelete( pLock->mutex );
	free( pLock );
}

void __retarget_lock_close_recursive( _LOCK_T pLock )
{
	assert( pLock );

	vSemaphoreDelete( pLock->mutex );
	free( pLock );
}

void __retarget_lock_acquire( _LOCK_T pLock )
{
	BaseType_t ret;

	assert( pLock );

	do
	{
		ret = xSemaphoreTake( pLock->mutex, portMAX_DELAY );
	} while( ret != pdTRUE );
}

void __retarget_lock_acquire_recursive( _LOCK_T pLock )
{
	BaseType_t ret;

	assert( pLock );

	do
	{
		ret = xSemaphoreTakeRecursive( pLock->mutex, portMAX_DELAY );
	} while( ret != pdTRUE );
}

int __retarget_lock_try_acquire( _LOCK_T pLock )
{
	BaseType_t ret;

	assert( pLock );

	ret = xSemaphoreTake( pLock->mutex, 0 );
	return (ret == pdTRUE);
}

int __retarget_lock_try_acquire_recursive( _LOCK_T pLock )
{
	BaseType_t ret;

	assert( pLock );

	ret = xSemaphoreTakeRecursive( pLock->mutex, 0 );
	return (ret == pdTRUE);
}

void __retarget_lock_release( _LOCK_T pLock )
{
	assert( pLock );

	xSemaphoreGive( pLock->mutex );
}

void __retarget_lock_release_recursive( _LOCK_T pLock )
{
	assert( pLock );

	xSemaphoreGiveRecursive( pLock->mutex );
}

void newlibLockInit( void )
{
	__lock___sinit_recursive_mutex.mutex	= xSemaphoreCreateRecursiveMutexStatic( &__lock___sinit_recursive_mutex.mutexStorage );
	__lock___sfp_recursive_mutex.mutex		= xSemaphoreCreateRecursiveMutexStatic( &__lock___sfp_recursive_mutex.mutexStorage );
	__lock___atexit_recursive_mutex.mutex	= xSemaphoreCreateRecursiveMutexStatic( &__lock___atexit_recursive_mutex.mutexStorage );
	__lock___at_quick_exit_mutex.mutex		= xSemaphoreCreateRecursiveMutexStatic( &__lock___at_quick_exit_mutex.mutexStorage );
	__lock___malloc_recursive_mutex.mutex	= xSemaphoreCreateRecursiveMutexStatic( &__lock___malloc_recursive_mutex.mutexStorage );
	__lock___env_recursive_mutex.mutex		= xSemaphoreCreateRecursiveMutexStatic( &__lock___env_recursive_mutex.mutexStorage );
	__lock___tz_mutex.mutex					= xSemaphoreCreateRecursiveMutexStatic( &__lock___tz_mutex.mutexStorage );
	__lock___dd_hash_mutex.mutex			= xSemaphoreCreateRecursiveMutexStatic( &__lock___dd_hash_mutex.mutexStorage );
	__lock___arc4random_mutex.mutex			= xSemaphoreCreateRecursiveMutexStatic( &__lock___arc4random_mutex.mutexStorage );
}
