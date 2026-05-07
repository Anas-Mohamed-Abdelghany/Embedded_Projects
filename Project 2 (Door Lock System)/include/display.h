#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/* 7-segment: show a single digit 0-9 (or blank for >9) */
void     SevenSeg_ShowDigit(uint8_t digit);
void     SevenSeg_Clear(void);

/* LED bar: light up 'count' LEDs (0-4) to show progress */
void     LEDBar_SetProgress(uint8_t count);

/* Status LEDs */
void     LED_SetSuccess(uint8_t on);
void     LED_SetAlarm(uint8_t on);
void     LED_PulseBell(void);          /* non-blocking pulse start */
void     LED_BellTick(void);           /* call from SysTick/main loop */

/* Init */
void     Display_Init(void);

#endif /* DISPLAY_H */
