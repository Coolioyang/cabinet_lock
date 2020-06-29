
#include "typedef.h"

#include "stm32l4xx_hal.h"

#include "24cxx_config.h"

#include "24Cxx.h"

#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c1;

#define EEPROM_ADDR     0xA0

#define I2C_TIMEOUT     20

static bool Write24CxxByteInternal(uint8_t devAddr, uint16_t addr, uint8_t addrLen, uint8_t dat)
{
	if (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, addr, addrLen, &dat, 1, I2C_TIMEOUT) == HAL_OK)
	{
		//delay some time to complete
		osDelay(10 * configTICK_RATE_HZ / 1000);

		return true;
	}
	
	return false;	
}

/****************************************************************
Description:  Write a byte at a specified address
Input:
addr :        Address to write at 
dat  : 		  Byte to write

Output:       true :success
			  false:failure
****************************************************************/
bool Write24CxxByte(uint16_t addr, uint8_t dat)
{
	uint8_t devAddr = EEPROM_ADDR;
	uint8_t addrLen;
	
#if EEPROM_TYPE == EEPROM_TYPE_24C01 || EEPROM_TYPE == EEPROM_TYPE_24C02
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C04
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2));
	
	devAddr |= (((addr >> 8) & 0x01) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C08
	devAddr |= (EEPROM_PIN_A2 << 3);
	
	devAddr |= (((addr >> 8) & 0x03) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C16
	devAddr |= (((addr >> 8) & 0x07) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C32 || EEPROM_TYPE == EEPROM_TYPE_24C64
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 2;

#elif EEPROM_TYPE == EEPROM_TYPE_24C512
	devAddr |= ((EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));

	addrLen = 2;
#endif
	
	return Write24CxxByteInternal(devAddr, addr, addrLen, dat);	
}

static bool Read24CxxByteInternal(uint8_t devAddr, uint16_t addr, uint8_t addrLen, uint8_t *pData)
{
	uint8_t dat;
	if (HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, addr, addrLen, &dat, 1, I2C_TIMEOUT) == HAL_OK)
	{
		if (pData != NULL)
		{
			*pData = dat;
		}

		return true;
	}

	return false;	
}

/****************************************************************
Description:  Read a byte from a specified address
Input:
addr :        Address to read from 
pData: 		  Byte to read

Output:       true :success
			  false:failure
****************************************************************/
bool Read24CxxByte(uint16_t addr, uint8_t *pData)
{
	uint8_t devAddr = EEPROM_ADDR;
	uint8_t addrLen;
	
#if EEPROM_TYPE == EEPROM_TYPE_24C01 || EEPROM_TYPE == EEPROM_TYPE_24C02
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C04
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2));
	
	devAddr |= (((addr >> 8) & 0x01) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C08
	devAddr |= (EEPROM_PIN_A2 << 3);
	
	devAddr |= (((addr >> 8) & 0x03) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C16
	devAddr |= (((addr >> 8) & 0x07) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C32 || EEPROM_TYPE == EEPROM_TYPE_24C64
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 2;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C512
	devAddr |= ((EEPROM_PIN_A1 << 2)
					|(EEPROM_PIN_A0 << 1));
	
	addrLen = 2;

#endif

	return Read24CxxByteInternal(devAddr, addr, addrLen, pData);	
}

static bool Write24CxxDataPageInternal(uint8_t devAddr, uint16_t addr, uint8_t addrLen, const uint8_t *pData, uint8_t size)
{
	if (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, addr, addrLen, (uint8_t *)pData, size, I2C_TIMEOUT) == HAL_OK)
	{
		//delay some time to complete
		osDelay(10 * configTICK_RATE_HZ / 1000);

		return true;
	}
	
	return false;
}

static bool Write24CxxDataPage(uint16_t addr, const uint8_t *pData, uint8_t size)
{
	uint8_t devAddr = EEPROM_ADDR;
	uint8_t addrLen;
	
#if EEPROM_TYPE == EEPROM_TYPE_24C01 || EEPROM_TYPE == EEPROM_TYPE_24C02
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C04
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2));
	
	devAddr |= (((addr >> 8) & 0x01) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C08
	devAddr |= (EEPROM_PIN_A2 << 3);
	
	devAddr |= (((addr >> 8) & 0x03) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C16
	devAddr |= (((addr >> 8) & 0x07) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C32 || EEPROM_TYPE == EEPROM_TYPE_24C64
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 2;

#elif EEPROM_TYPE == EEPROM_TYPE_24C512
	devAddr |= ((EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));

	addrLen = 2;
#endif
	
	return Write24CxxDataPageInternal(devAddr, addr, addrLen, pData, size);
}

/****************************************************************
Description:  Write data from a specified address
Input:
addr :        Address to write from
pData:        Data to write
size : 		  Count of data in byte

Output:       true :success
			  false:failure
****************************************************************/
bool Write24CxxData(uint16_t addr, const uint8_t *pData, uint16_t size)
{
	uint16_t left = size;
	
	uint16_t pageOffset = addr % EEPROM_PAGE_SIZE;

	if (pageOffset > 0)
	{
		uint16_t pageRemain = EEPROM_PAGE_SIZE - pageOffset;

		if (pageRemain > left)
		{
			pageRemain = left;
		}

		if (!Write24CxxDataPage(addr, pData, pageRemain))
		{
			return false;
		}
		
		left -= pageRemain;
		pData += pageRemain;
		addr += pageRemain;
	}
	
	while (left >= EEPROM_PAGE_SIZE)
	{
		if (!Write24CxxDataPage(addr, pData, EEPROM_PAGE_SIZE))
		{
			return false;
		}
			
		pData += EEPROM_PAGE_SIZE;
		addr += EEPROM_PAGE_SIZE;
		
		left -= EEPROM_PAGE_SIZE;
	}
	
	if (left > 0)
	{
		if (!Write24CxxDataPage(addr, pData, left))
		{
			return false;
		}
	}
	
	return true;	
}

static bool Read24CxxDataInternal(uint8_t devAddr, uint16_t addr, uint8_t addrLen, uint8_t *pData, uint16_t size, uint16_t *pSizeRet)
{
	if (HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, addr, addrLen, pData, size, I2C_TIMEOUT) == HAL_OK)
	{
		if (pSizeRet != NULL)
		{
			*pSizeRet = size;
		}

		return true;
	}

	return false;	
}

/****************************************************************
Description:  Read data from a specified address
Input:
addr :        Address to read from
pData:        Buffer to read data
size : 		  Count of data in byte to read
pSizeRet:     Count of data that has read

Output:       true :success
			  false:failure
****************************************************************/
bool Read24CxxData(uint16_t addr, uint8_t *pData, uint16_t size, uint16_t *pSizeRet)
{
	uint8_t devAddr = EEPROM_ADDR;
	uint8_t addrLen;
	
#if (EEPROM_TYPE == EEPROM_TYPE_24C01) || (EEPROM_TYPE == EEPROM_TYPE_24C02)
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C04
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2));
	
	devAddr |= (((addr >> 8) & 0x01) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C08
	devAddr |= (EEPROM_PIN_A2 << 3);
	
	devAddr |= (((addr >> 8) & 0x03) << 1);
	
	addrLen = 1;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C16
	devAddr |= (((addr >> 8) & 0x07) << 1);
	
	addrLen = 1;
	
#elif (EEPROM_TYPE == EEPROM_TYPE_24C32) || (EEPROM_TYPE == EEPROM_TYPE_24C64)
	devAddr |= ((EEPROM_PIN_A2 << 3)
				|(EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));
	
	addrLen = 2;
	
#elif EEPROM_TYPE == EEPROM_TYPE_24C512
	devAddr |= ((EEPROM_PIN_A1 << 2)
				|(EEPROM_PIN_A0 << 1));

	addrLen = 2;
#endif
	
	return 	Read24CxxDataInternal(devAddr, addr, addrLen, pData, size, pSizeRet);	
}

