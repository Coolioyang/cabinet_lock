#include <stdbool.h> 
#include "stm32l4xx_hal.h"

#include "Gatein.h"

#include "cmsis_os.h"
#include "timers.h"

#include "main_looper.h"

#define GPIO_GATE           GPIOA
#define PIN_GATE            GPIO_PIN_11

/* gate status */
typedef enum
{
	GATE_STATUS_IDLE = 0,
	GATE_STATUS_BOUND,
} GATE_STATUS;

#define GATE_OPEN_PIN_STATE      GPIO_PIN_SET
#define GATE_CLOSED_PIN_STATE    GPIO_PIN_RESET

/* gate count */
#define GATE_CHECK_COUNT    (50 / 10)

static GPIO_PinState CheckGate(void)
{
	return HAL_GPIO_ReadPin(GPIO_GATE, PIN_GATE);
}

static void EnableGateIrq(bool enable)
{
	if (enable)
	{
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	}
	else
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	}
}

typedef struct
{
	GATE_STATUS gateStatus;
	
    uint8_t  gate;

	uint8_t  count;
	
	GPIO_PinState oldStatus;

	GPIO_PinState (*CheckFun)(void);

	void (*EnableIrq)(bool);

	TimerHandle_t timer;

} GATE_CHECK;

static GATE_CHECK s_gateCheck[GATE_NUM];

#define GATE_BUFFER_SIZE     8

static void (*s_gateInChangedCb)(uint8_t gate, uint16_t status) = NULL;

static void GateInHandler(void *param)
{
	uint16_t gateIn = (uint16_t)(uint32_t)param;
	if (s_gateInChangedCb)
	{
		s_gateInChangedCb(GATE_VALUE(gateIn), GATE_TYPE(gateIn));
	}
}

static void TimerCallback(TimerHandle_t hTimer) 
{
	GATE_CHECK *pGateCheck = (GATE_CHECK *)pvTimerGetTimerID (hTimer);
	
	uint16_t gate = (uint16_t)GATE_NONE;
	
    GPIO_PinState status = pGateCheck->CheckFun();
	if (pGateCheck->oldStatus != status)
	{
		switch (pGateCheck->gateStatus)
		{
		case GATE_STATUS_IDLE:
			pGateCheck->gateStatus = GATE_STATUS_BOUND;
				
			pGateCheck->count = GATE_CHECK_COUNT;
			
			break;
			
		case GATE_STATUS_BOUND:
			pGateCheck->count--;
			if (pGateCheck->count == 0)
			{
                if (status == GATE_OPEN_PIN_STATE)
                {
                    gate = pGateCheck->gate | MASK_GATE_OPEN;
                }
                else
                {
                    gate = pGateCheck->gate | MASK_GATE_CLOSED;
                }
                
                pGateCheck->oldStatus = status;
					
				pGateCheck->gateStatus = GATE_STATUS_IDLE;

                xTimerStop(pGateCheck->timer, 0);

		        pGateCheck->EnableIrq(true);
			}

			break;
			
		default:
			break;
		}
	}
	else
	{		
		pGateCheck->count = 0;
		pGateCheck->gateStatus = GATE_STATUS_IDLE;

		xTimerStop(pGateCheck->timer, 0);

		pGateCheck->EnableIrq(true);
	}

	if (gate != (uint16_t)GATE_NONE)
	{
		//gate status changed
		main_looper_send_msg(GateInHandler, (void *)gate);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (GPIO_Pin == GPIO_PIN_11)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

		xTimerStartFromISR(s_gateCheck[0].timer, &xHigherPriorityTaskWoken);
	}

	if( xHigherPriorityTaskWoken != pdFALSE)
	{
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void InitGate(void)
{
    s_gateCheck[0].gateStatus    = GATE_STATUS_IDLE;
    s_gateCheck[0].gate          = GATE_1;
	s_gateCheck[0].count         = 0;
    s_gateCheck[0].oldStatus     = CheckGate();
	s_gateCheck[0].CheckFun      = CheckGate;	
	s_gateCheck[0].timer         = xTimerCreate(NULL, 10 * configTICK_RATE_HZ / 1000, pdTRUE, &s_gateCheck[0], TimerCallback);
	s_gateCheck[0].EnableIrq     = EnableGateIrq;

    s_gateCheck[0].EnableIrq(true);
}

void SetGateInChangedCallback(void (*cb)(uint8_t, uint16_t))
{
	s_gateInChangedCb = cb;
}
