#include "typedef.h"

#include "stm32l4xx_hal.h"

#define GPIO_BOOST       GPIOA
#define PIN_BOOST        GPIO_PIN_9

void EnableBoost(bool enable)
{
    GPIO_PinState pinState = GPIO_PIN_RESET;
    if (enable)
    {
        pinState = GPIO_PIN_SET;
    }
    
    HAL_GPIO_WritePin(GPIO_BOOST, PIN_BOOST, pinState);
}
