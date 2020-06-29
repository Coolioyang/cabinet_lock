#pragma once

extern bool Write24CxxByte(uint16_t addr, uint8_t dat);
extern bool Read24CxxByte(uint16_t addr, uint8_t *pData);

extern bool Write24CxxData(uint16_t addr, const uint8_t *pData, uint16_t size);
extern bool Read24CxxData(uint16_t addr, uint8_t *pData, uint16_t size, uint16_t *pSizeRet);


