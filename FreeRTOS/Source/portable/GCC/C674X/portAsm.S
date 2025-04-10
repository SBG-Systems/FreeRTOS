#include "portAsm.h"

.func vPortStartFirstTask
.global vPortStartFirstTask
.p2align 5
vPortStartFirstTask:
	portRESTORE_CONTEXT nrp, ntsr
