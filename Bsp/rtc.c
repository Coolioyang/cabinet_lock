
#include "typedef.h"

#include "stm32l4xx_hal.h"
#include "rtc.h"

#define ISL1208_ADDRESS   0xDE

//registers
//rtc section
#define ISL1208_REG_SC      0x00
#define ISL1208_REG_MN      0x01
#define ISL1208_REG_HR      0x02
#define ISL1208_REG_HR_MIL  (1 << 7)  /* 24h/12h mode */
#define ISL1208_REG_HR_PM   (1 << 5)  /* PM/AM bit in 12h mode */
#define ISL1208_REG_DT      0x03
#define ISL1208_REG_MO      0x04
#define ISL1208_REG_YR      0x05
#define ISL1208_REG_DW      0x06
#define ISL1208_RTC_SECTION_LEN  7

//control status section
#define ISL1208_REG_SR      0x07
#define ISL1208_REG_SR_ARST (1 << 7) /* auto reset */
#define ISL1208_REG_SR_XTOSCB (1 << 6) /* crystal oscillator */
#define ISL1208_REG_SR_WRTC (1 << 4) /* write rtc */
#define ISL1208_REG_SR_ALM  (1 << 2) /* alarm */
#define ISL1208_REG_SR_BAT  (1 << 1) /* battery */
#define ISL1208_REG_SR_RTCF (1 << 0) /* rtc failed */
#define ISL1208_REG_INT     0x08
#define ISL1208_REG_INT_ALME (1 << 6) /* alarm enable */
#define ISL1208_REG_INT_IM  (1 << 7)  /* interrupt/alarm mode */
#define ISL1208_REG_ATR     0x0A
#define ISL1208_REG_DTR     0x0B

//alarm section
#define ISL1208_REG_SCA     0x0C
#define ISL1208_REG_MNA     0x0D
#define ISL1208_REG_HRA     0x0E
#define ISL1208_REG_DTA     0x0F
#define ISL1208_REG_MOA     0x10
#define ISL1208_REG_DWA     0x11
#define ISL1208_ALARM_SECTION_LEN  6

//user section
#define ISL1208_REG_USER1   0x12
#define ISL1208_REG_USER2   0x13
#define ISL1208_USER_SECTION_LEN   2

extern I2C_HandleTypeDef hi2c1;

//default time
static const TimeStruct s_defaultTime = 
{
	.second = 0,
	.minute = 0,
	.hour   = 0,
	.year   = 2000,
	.month  = 1,
	.day    = 1,
	.dayOfWeek = 0,
};

static uint8_t IntToBcd(uint8_t d)
{
	return ((d / 10) << 4) + ((d % 10) & 0x0f);
}

static uint8_t BcdToInt(uint8_t bcd)
{ 
	return ((bcd >> 4) & 0x0f) * 10 + (bcd & 0x0f);
}

bool IsValidRTCTime(const TimeStruct *time)
{
    if (time->second > 59
		|| time->minute > 59
		|| time->hour > 23
		|| time->day < 1 || time->day > 31
		|| time->month < 1 || time->month > 12 
		|| time->dayOfWeek > 6
		|| time->year < 2000 || time->year > 2000 + 99)
	{
		return false;
	}

	return true;
}

bool SetDefaultRTCTime(void)
{
    return SetRTCTime(&s_defaultTime);
}

bool SetRTCTime(const TimeStruct *time)
{
	if (!IsValidRTCTime(time))
	{
		return false;
	}
	
	uint8_t dat[ISL1208_RTC_SECTION_LEN + 1];
	
	//enable write rtc
    dat[0] = ISL1208_REG_SR;
    dat[1] = ISL1208_REG_SR_ARST | ISL1208_REG_SR_WRTC;
	HAL_I2C_Master_Transmit(&hi2c1, ISL1208_ADDRESS, dat, 2, 10);
	
    dat[0] = ISL1208_REG_SC;
    dat[1] = IntToBcd(time->second);
	dat[2] = IntToBcd(time->minute);
	dat[3] = IntToBcd(time->hour) | ISL1208_REG_HR_MIL;
	dat[4] = IntToBcd(time->day);
	dat[5] = IntToBcd(time->month);
	dat[6] = IntToBcd(time->year - 2000);
	dat[7] = IntToBcd(time->dayOfWeek & 7);

	HAL_I2C_Master_Transmit(&hi2c1, ISL1208_ADDRESS, dat, 8, 10);

	//disable write rtc
    dat[0] = ISL1208_REG_SR;
    dat[1] = ISL1208_REG_SR_ARST;
	HAL_I2C_Master_Transmit(&hi2c1, ISL1208_ADDRESS, dat, 8, 10);

	return true;
}

bool GetRTCTime(TimeStruct *time)
{
	uint8_t reg;
	uint8_t dat[ISL1208_RTC_SECTION_LEN];
	reg = ISL1208_REG_SC;
	if (HAL_I2C_Mem_Read(&hi2c1, ISL1208_ADDRESS, reg, 1, dat, ISL1208_RTC_SECTION_LEN, 10) == HAL_OK)
	{
		if (time)
		{
			time->second = BcdToInt(dat[ISL1208_REG_SC]);
			time->minute = BcdToInt(dat[ISL1208_REG_MN]);
			if (dat[ISL1208_REG_HR] & ISL1208_REG_HR_MIL)
			{
				/* 24h format */
				time->hour = BcdToInt(dat[ISL1208_REG_HR] & 0x3f);
			}
			else
			{
				/* 12h format */
				time->hour = BcdToInt(dat[ISL1208_REG_HR] & 0x1f);
				if (dat[ISL1208_REG_HR] & ISL1208_REG_HR_PM)
				{
					/* PM flag */
					time->hour += 12;
				}
			}

			time->day       = BcdToInt(dat[ISL1208_REG_DT]);
			time->month     = BcdToInt(dat[ISL1208_REG_MO]);
			time->year      = BcdToInt(dat[ISL1208_REG_YR]) + 2000;
			time->dayOfWeek = BcdToInt(dat[ISL1208_REG_DW]);
		}
		
		return true;
	}
	
	return false;
}
