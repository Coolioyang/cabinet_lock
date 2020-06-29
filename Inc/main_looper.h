#pragma once

extern BaseType_t main_looper_init(void);

extern void main_looper_loop(void);

extern BaseType_t main_looper_send_msg(void (*handler)(void *), void *context);
extern BaseType_t main_looper_urgent_msg(void (*handler)(void *), void *context);

extern BaseType_t main_looper_send_msg_from_isr(void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken);
extern BaseType_t main_looper_urgent_msg_from_isr(void (*handler)(void *), void *context, BaseType_t *const pxHigherPriorityTaskWoken);
