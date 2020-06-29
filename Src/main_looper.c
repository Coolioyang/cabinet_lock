#include "FreeRTOS.h"
#include "task.h"

#include "looper.h"

#define MAIN_LOOPER_MSG_QUEUE_LEN    16

static msg_looper *s_main_looper = NULL;

BaseType_t main_looper_init(void)
{
    s_main_looper = msg_looper_create( MAIN_LOOPER_MSG_QUEUE_LEN);
	if (s_main_looper == NULL)
    {
        return pdFALSE;
    }

    return pdTRUE;
}

void main_looper_loop(void)
{
    configASSERT(s_main_looper != NULL);

    msg_looper_loop(s_main_looper, portMAX_DELAY);
}

BaseType_t main_looper_send_msg(void (*handler)(void *), void *context)
{
    configASSERT(s_main_looper != NULL);

    return msg_looper_send_msg(s_main_looper, handler, context);
}

BaseType_t main_looper_urgent_msg(void (*handler)(void *), void *context)
{
    configASSERT(s_main_looper != NULL);

    return msg_looper_urgent_msg(s_main_looper, handler, context);
}

BaseType_t main_looper_send_msg_from_isr(void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken)
{
    configASSERT(s_main_looper != NULL);

    return msg_looper_send_msg_from_isr(s_main_looper, handler, context, pxHigherPriorityTaskWoken);
}

BaseType_t main_looper_urgent_msg_from_isr(void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken)
{
    configASSERT(s_main_looper != NULL);

    return msg_looper_urgent_msg_from_isr(s_main_looper, handler, context, pxHigherPriorityTaskWoken);
}
