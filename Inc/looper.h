#pragma once

struct _msg_looper;
typedef struct _msg_looper msg_looper;

extern BaseType_t msg_looper_init(msg_looper *looper, UBaseType_t max_msgs);
extern void msg_looper_detach(msg_looper *looper);

extern msg_looper *msg_looper_create(UBaseType_t max_msgs);
extern void msg_looper_delete(msg_looper *looper);

extern void msg_looper_loop(msg_looper *looper, TickType_t timeout);

extern BaseType_t msg_looper_send_msg(msg_looper *looper, void (*handler)(void *), void *context);
extern BaseType_t msg_looper_urgent_msg(msg_looper *looper, void (*handler)(void *), void *context);

extern BaseType_t msg_looper_send_msg_from_isr(msg_looper *looper, void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken);
extern BaseType_t msg_looper_urgent_msg_from_isr(msg_looper *looper, void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken);
