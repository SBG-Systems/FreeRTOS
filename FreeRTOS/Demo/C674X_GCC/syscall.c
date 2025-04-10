#include <errno.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <FreeRTOS.h>
#include <task.h>

#include "newlibLock.h"
#include "syscall.h"

/*
 * System call context.
 *
 * Heap resizes are synchronized by disabling preemption.
 */
typedef struct _SyscallContext
{
	bool initialized;

	size_t heapSize;
	size_t heapPeakSize;
} SyscallContext;

/*
 * Global system call context singleton instance.
 */
static SyscallContext syscallContext;

/*
 * Memory reserved for the heap.
 */
static alignas(8) uint8_t syscallHeap[ configTOTAL_HEAP_SIZE ] __attribute__((section(".heap")));

/*
 * Check if a syscall context has been initialized.
 */
static bool syscallContextInitialized( const SyscallContext *pContext )
{
	assert( pContext );

	return pContext->initialized;
}

/*
 * Construct a syscall context.
 */
static void syscallContextConstruct( SyscallContext *pContext )
{
	assert( ! syscallContextInitialized( pContext ) );

	pContext->heapSize		= 0;
	pContext->heapPeakSize	= 0;

	pContext->initialized = true;
}

/*
 * Increase a syscall context data space.
 */
static void *syscallContextSbrk( SyscallContext *pContext, intptr_t increment )
{
	uint8_t *pPrevEnd;
	size_t size;

	assert( pContext );

	vTaskSuspendAll();

	size = pContext->heapSize + increment;

	if( size <= sizeof( syscallHeap ) )
	{
		pPrevEnd = &syscallHeap[ pContext->heapSize ];

		if( increment != 0 )
		{
			pContext->heapSize = size;

			if( pContext->heapSize > pContext->heapPeakSize )
			{
				pContext->heapPeakSize = pContext->heapSize;
			}
		}
	}
	else
	{
		pPrevEnd = ( void * ) -1;
	}

	xTaskResumeAll();

	return pPrevEnd;
}

/*
 * Get the syscall context.
 */
static SyscallContext *syscallGetContext( void )
{
	SyscallContext *pContext;

	pContext = &syscallContext;

	assert(syscallContextInitialized(pContext));
	return pContext;
}

/* This is the list of system calls required by Newlib and libstdc++. */
void *	__attribute__((externally_visible)) _sbrk		( intptr_t increment );
int		__attribute__((externally_visible)) _getpid		( void );
int		__attribute__((externally_visible)) _kill		( int pid, int sig );
void	__attribute__((externally_visible)) _exit		( int status );
int		__attribute__((externally_visible)) _open		( const char *pPath, int flags, int mode );
int		__attribute__((externally_visible)) _close		( int fd );
ssize_t	__attribute__((externally_visible)) _read		( int fd, void *pBuffer, size_t size );
ssize_t	__attribute__((externally_visible)) _write		( int fd, const void *pBuffer, size_t size );
off_t	__attribute__((externally_visible)) _lseek		( int fd, off_t offset, int whence );
int		__attribute__((externally_visible)) _fstat		( int fd, struct stat *pStat );
int		__attribute__((externally_visible)) _isatty		( int fd );
int		__attribute__((externally_visible)) _link		( const char *pOldPath, const char *pNewPath );
int		__attribute__((externally_visible)) _unlink		( const char *pPath );
ssize_t	__attribute__((externally_visible)) read		( int fd, void *pBuffer, size_t size );
int		__attribute__((externally_visible)) getentropy	( void *pBuffer, size_t size );

/* Some functions aren't strictly system calls, and instead override weak
definitions of the C library. */
void	__attribute__((externally_visible)) __assert_func	( const char *pFile, int line, const char *pFunc, const char *pExpr );

/*
 * Increase data space.
 */
void * _sbrk( intptr_t increment )
{
	uint8_t *pPrevEnd;

	pPrevEnd = syscallContextSbrk( syscallGetContext(), increment );

	if( pPrevEnd == ( void * ) -1 )
	{
		errno = ENOMEM;
	}

	return pPrevEnd;
}

/*
 * Get current PID.
 */
int _getpid( void )
{
	return 1;
}

/*
 * Send signal to a process.
 */
int _kill( int pid, int sig )
{
	( void ) pid;
	( void ) sig;

	errno = ENOSYS;
	return -1;
}

/*
 * Exit a task.
 */
void _exit( int status )
{
	( void ) status;

	portDISABLE_INTERRUPTS();

	for( ;; );
}

int _open( const char *pPath, int flags, int mode )
{
	( void ) pPath;
	( void ) flags;
	( void ) mode;

	errno = ENOSYS;
	return -1;
}

int _close( int fd )
{
	( void ) fd;

	errno = ENOSYS;
	return -1;
}

ssize_t _read( int fd, void *pBuffer, size_t size )
{
	( void ) fd;
	( void ) pBuffer;
	( void ) size;

	errno = ENOSYS;
	return -1;
}

ssize_t _write( int fd, const void *pBuffer, size_t size )
{
	( void ) fd;
	( void ) pBuffer;
	( void ) size;

	errno = ENOSYS;
	return -1;
}

off_t _lseek( int fd, off_t offset, int whence )
{
	( void ) fd;
	( void ) offset;
	( void ) whence;

	errno = ENOSYS;
	return -1;
}

int _fstat( int fd, struct stat *pStat )
{
	( void ) fd;
	( void ) pStat;

	errno = ENOSYS;
	return -1;
}

int _isatty( int fd )
{
	( void ) fd;

	errno = ENOSYS;
	return -1;
}

int _link( const char *pOldPath, const char *pNewPath )
{
	( void ) pOldPath;
	( void ) pNewPath;

	errno = ENOSYS;
	return -1;
}

int _unlink( const char *pPath )
{
	( void ) pPath;

	errno = ENOSYS;
	return -1;
}

ssize_t read( int fd, void *pBuffer, size_t size )
{
	return _read(fd, pBuffer, size);
}

int getentropy( void *pBuffer, size_t size )
{
	( void ) pBuffer;
	( void ) size;

	errno = ENOSYS;
	return -1;
}

void __assert_func( const char *pFile, int line, const char *pFunc, const char *pExpr )
{
	( void ) pFile;
	( void ) line;
	( void ) pFunc;
	( void ) pExpr;

	portDISABLE_INTERRUPTS();

	for( ;; );
}

void syscallInit(void)
{
	newlibLockInit();
	syscallContextConstruct( &syscallContext );
}
