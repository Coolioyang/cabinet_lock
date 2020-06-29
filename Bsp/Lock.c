#include "typedef.h"

#include "stm32l4xx_hal.h"

#define GPIO_LOCK       GPIOA
#define PIN_LOCK        GPIO_PIN_10

void TurnOnLock(bool on)
{
    GPIO_PinState pinState = GPIO_PIN_RESET;
    if (on)
    {
        pinState = GPIO_PIN_SET;
    }
    
    HAL_GPIO_WritePin(GPIO_LOCK, PIN_LOCK, pinState);
}
