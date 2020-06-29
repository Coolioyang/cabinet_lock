#pragma once

#define LOCK_ON     1
#define LOCK_OFF    0

typedef struct 
{
    uint8_t  action;  //动作

    uint32_t boost_action_duration;  //加压持续时间
} LockAction;

extern void InitLockTask(void);

extern void SetLockActionCompleteCallback(void (*cb)(uint8_t));

extern bool OperateLock(uint8_t action, uint32_t boostDuration);
