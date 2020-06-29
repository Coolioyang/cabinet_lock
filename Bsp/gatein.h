#pragma once

#define GATE_NUM   1

#define GATE_1      0x00

#define GATE_NONE   0xff

/* key mask */
#define MASK_GATE_OPEN    0x0100
#define MASK_GATE_CLOSED  0x0200

#define GATE_VALUE(gate)    ((gate) & 0x00ff)
#define GATE_TYPE(gate)     ((gate) & 0xff00)

extern void InitGate(void);

extern void SetGateInChangedCallback(void (*cb)(uint8_t, uint16_t));

