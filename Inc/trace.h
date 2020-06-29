#ifndef _TRACE_H_
#define _TRACE_H_

#include "SEGGER_RTT.h"

extern void trace_init(void);

#define TRACE(fmt, ...)  SEGGER_RTT_printf(0, fmt, ##__VA_ARGS__)

#endif
