#pragma once

#define EEPROM_TYPE_24C01  0
#define EEPROM_TYPE_24C02  1
#define EEPROM_TYPE_24C04  2
#define EEPROM_TYPE_24C08  3
#define EEPROM_TYPE_24C16  4
#define EEPROM_TYPE_24C32  5
#define EEPROM_TYPE_24C64  6

#define EEPROM_TYPE_24C512 8

#define EEPROM_TYPE    EEPROM_TYPE_24C512

#if (EEPROM_TYPE == EEPROM_TYPE_24C01) || (EEPROM_TYPE == EEPROM_TYPE_24C02)
#define EEPROM_PIN_A2     0
#define EEPROM_PIN_A1     0
#define EEPROM_PIN_A0     0

#define EEPROM_PAGE_SIZE         8

#elif EEPROM_TYPE == EEPROM_TYPE_24C04
#define EEPROM_PIN_A2     1
#define EEPROM_PIN_A1     1

#define EEPROM_PAGE_SIZE         16

#elif EEPROM_TYPE == EEPROM_TYPE_24C08
#define EEPROM_PIN_A2     1

#define EEPROM_PAGE_SIZE         16

#elif EEPROM_TYPE == EEPROM_TYPE_24C16
#define EEPROM_PAGE_SIZE         16

#elif (EEPROM_TYPE == EEPROM_TYPE_24C32) || (EEPROM_TYPE == EEPROM_TYPE_24C64)
#define EEPROM_PIN_A2     0
#define EEPROM_PIN_A1     0
#define EEPROM_PIN_A0     1

#define EEPROM_PAGE_SIZE         32

#elif ((EEPROM_TYPE == EEPROM_TYPE_24C512))
#define EEPROM_PIN_A1     0
#define EEPROM_PIN_A0     0

#define EEPROM_PAGE_SIZE         128

#else

#error  "device type error"

#endif 

