#pragma once

//time struct
typedef struct
{
	uint8_t second;  // 0-59
	uint8_t minute;  // 0-59
	uint8_t hour;     // 0-23
	uint8_t day;      // 1-31
	uint8_t month;    // 1-12
	uint8_t dayOfWeek; //0-6
	uint16_t year;    // 2000+
} TimeStruct;

extern bool IsValidRTCTime(const TimeStruct *time);

extern bool SetDefaultRTCTime(void);

extern bool SetRTCTime(const TimeStruct *time);

extern bool GetRTCTime(TimeStruct *time);


