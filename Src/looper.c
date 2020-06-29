#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "looper.h"

struct _msg_looper
{
	QueueHandle_t msg_queue;
};

typedef struct
{
	void (*handler)(void *);

	void *context;
} msg_handler;

static BaseType_t _msg_looper_init(msg_looper *looper, UBaseType_t max_msgs)
{
    configASSERT(looper != NULL);

    QueueHandle_t q = xQueueCreate(sizeof(msg_handler), max_msgs);
    if (q == NULL)
    {
        return pdFALSE;
    }

    looper->msg_queue = q;

    return pdTRUE;
}

BaseType_t msg_looper_init(msg_looper *looper, UBaseType_t max_msgs)
{
    return _msg_looper_init(looper, max_msgs);
}

void msg_looper_detach(msg_looper *looper)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    vQueueDelete(looper->msg_queue);
}

msg_looper *msg_looper_create(UBaseType_t max_msgs)
{
    msg_looper *looper = pvPortMalloc(sizeof(msg_looper));
    if (looper == NULL)
    {
        return NULL;
    }

    if (msg_looper_init(looper, max_msgs) == pdFALSE)
    {
        vPortFree(looper);

        return NULL;
    }

    return looper;
}

void msg_looper_delete(msg_looper *looper)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    vQueueDelete(looper->msg_queue);

    vPortFree(looper);
}

void msg_looper_loop(msg_looper *looper, TickType_t timeout)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    for ( ; ; )
    {
        msg_handler handler;
        BaseType_t result = xQueueReceive(looper->msg_queue, &handler, timeout);
        if (result != pdTRUE)
        {
            continue;
        }

        if (handler.handler != NULL)
        {
            handler.handler(handler.context);
        }
    }
}

BaseType_t msg_looper_send_msg(msg_looper *looper, void (*handler)(void *), void *context)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    msg_handler msg_handler;
	msg_handler.handler = handler;
	msg_handler.context = context;

    BaseType_t result = xQueueSend(looper->msg_queue, &msg_handler, 0);
    return result;
}

BaseType_t msg_looper_urgent_msg(msg_looper *looper, void (*handler)(void *), void *context)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    msg_handler msg_handler;
	msg_handler.handler = handler;
	msg_handler.context = context;

    BaseType_t result = xQueueSendToFront(looper->msg_queue, &msg_handler, 0);
    return result;   
}

BaseType_t msg_looper_send_msg_from_isr(msg_looper *looper, void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    msg_handler msg_handler;
	msg_handler.handler = handler;
	msg_handler.context = context;

    BaseType_t result = xQueueSendFromISR(looper->msg_queue, &msg_handler, pxHigherPriorityTaskWoken);
    return result;
}

BaseType_t msg_looper_urgent_msg_from_isr(msg_looper *looper, void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken)
{
    configASSERT(looper != NULL);
    configASSERT(looper->msg_queue != NULL);

    msg_handler msg_handler;
	msg_handler.handler = handler;
	msg_handler.context = context;

    BaseType_t result = xQueueSendToFrontFromISR(looper->msg_queue, &msg_handler, pxHigherPriorityTaskWoken);
    return result;  
}
