#include <stdbool.h>

#include "Lock.h"
#include "boost.h"

#include "cmsis_os.h"

#include "lock_task.h"

#include "trace.h"

typedef enum
{
    LOCK_STATUS_IDLE = 0,

    LOCK_STATUS_BOOSTING,
} LockStatus;

#define LOCK_TASK_MSG_COUNT   8

static LockStatus s_lockStatus = LOCK_STATUS_IDLE;

static osMessageQueueId_t s_mq = NULL;

osThreadId_t s_lockTaskHandle = NULL;

static const osThreadAttr_t s_lockTask_attributes = 
{
  .name = "Locktask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};

static void (*s_actionCompleteCallback)(uint8_t) = NULL;

static void LockTask(void *param)
{
    TickType_t timeout = osWaitForever;

    LockAction lockAction;

    for ( ; ; )
    {
        osStatus_t ret = osMessageQueueGet(s_mq, &lockAction, 0, timeout);
        if (ret == osOK)
        {
            //got an action cmd
            EnableBoost(lockAction.action == LOCK_ON);    

            timeout = lockAction.boost_action_duration * configTICK_RATE_HZ / 1000;

            s_lockStatus = LOCK_STATUS_BOOSTING;

            TRACE("action:%02d\n", lockAction.action);
        }
        else if (ret == osErrorTimeout)
        {
            if (s_lockStatus == LOCK_STATUS_BOOSTING)
            {
                TurnOnLock(lockAction.action == LOCK_ON);

                timeout = osWaitForever;

                s_lockStatus = LOCK_STATUS_IDLE;

                if (s_actionCompleteCallback)
                {
                    s_actionCompleteCallback(lockAction.action);
                }

                TRACE("action complete\n");
            }
        }
    }
}

void InitLockTask(void)
{
    s_lockStatus = LOCK_STATUS_IDLE;

    s_mq = osMessageQueueNew(LOCK_TASK_MSG_COUNT, sizeof(LockAction), NULL);

    s_lockTaskHandle = osThreadNew(LockTask, NULL, &s_lockTask_attributes);
}

void SetLockActionCompleteCallback(void (*cb)(uint8_t))
{
    s_actionCompleteCallback = cb;
}

bool OperateLock(uint8_t action, uint32_t boostDuration)
{
    LockAction lockAction;
    lockAction.action = action;
    lockAction.boost_action_duration = boostDuration;

    return osMessageQueuePut(s_mq, &lockAction, 0, 0) == osOK;
}
