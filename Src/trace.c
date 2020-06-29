#include "typedef.h"

#include "trace.h"

void trace_init(void)
{
	SEGGER_RTT_Init();
	
	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

