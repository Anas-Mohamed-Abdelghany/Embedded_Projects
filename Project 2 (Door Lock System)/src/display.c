#include "display.h"
#include "main.h"
#include "gpio.h" /* Your manual driver */
#include "rcc.h"  /* Your manual clock driver */

/* 7-Segment mapping These patterns assume Bit 0 = Segment A, Bit 1 = Segment B... */
static const uint8_t SEG_TABLE[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

/* Bell Pulse State */
extern uint32_t System_GetTick(void);
static uint32_t bell_pulse_start = 0;
static uint8_t  bell_active      = 0;

/* ─────────────── INIT ─────────────── */
void Display_Init(void)
{
    /* 1. Enable Clocks */
    Rcc_Enable('B');
    Rcc_Enable('C'); 
    Rcc_Enable('D');

    /* 2. Init 7-Segment Pins (PB0-PB6) as Output */
    for(uint8_t i = 0; i <= 6; i++) {
        Gpio_Init(SEG_PORT, i, GPIO_OUTPUT, HIGH); // Start HIGH (OFF for Common Anode)
    }

    /* 3. Init LED Bar Pins (PC0-PC3) as Output */
    Gpio_Init('C', 0,  GPIO_OUTPUT, LOW);
    Gpio_Init('C', 1,  GPIO_OUTPUT, LOW);
    Gpio_Init('C', 2, GPIO_OUTPUT, LOW);
    Gpio_Init('C', 3, GPIO_OUTPUT, LOW);

    /* 4. Init Status LEDs (PD0-PD2) as Output */
    Gpio_Init(LED_STATUS_PORT, LED_SUCCESS_PIN, GPIO_OUTPUT, LOW);
    Gpio_Init(LED_STATUS_PORT, LED_ALARM_PIN,   GPIO_OUTPUT, LOW);
    Gpio_Init(LED_STATUS_PORT, LED_BELL_PIN,    GPIO_OUTPUT, LOW);
}

/* ─────────────── 7-SEGMENT ─────────────── */
void SevenSeg_ShowDigit(uint8_t digit)
{
    if (digit > 9) {
        SevenSeg_Clear();
        return;
    }

    uint8_t bitmask = SEG_TABLE[digit];

    /* Extract bits and write to pins */
    for (uint8_t i = 0; i < 7; i++) {
        uint8_t segment_on = (bitmask >> i) & 0x01;
        
        /* Common Anode: If segment_on is 1, Pin must be LOW (0) */
        if (segment_on) {
            Gpio_WritePin(SEG_PORT, i, LOW);
        } else {
            Gpio_WritePin(SEG_PORT, i, HIGH);
        }
    }
}

void SevenSeg_Clear(void)
{
    for (uint8_t i = 0; i < 7; i++) {
        Gpio_WritePin(SEG_PORT, i, HIGH); // HIGH = OFF
    }
}

/* ─────────────── LED BAR ─────────────── */
void LEDBar_SetProgress(uint8_t count)
{
    // Pins on Port C
    Gpio_WritePin('C', 0,  (count >= 1) ? HIGH : LOW);
    Gpio_WritePin('C', 1,  (count >= 2) ? HIGH : LOW);
    Gpio_WritePin('C', 2, (count >= 3) ? HIGH : LOW);
    Gpio_WritePin('C', 3, (count >= 4) ? HIGH : LOW);
}

/* ─────────────── STATUS LEDs ─────────────── */
void LED_SetSuccess(uint8_t on) {
    Gpio_WritePin(LED_STATUS_PORT, LED_SUCCESS_PIN, on ? HIGH : LOW);
}

void LED_SetAlarm(uint8_t on) {
    Gpio_WritePin(LED_STATUS_PORT, LED_ALARM_PIN, on ? HIGH : LOW);
}

/* ─────────────── BELL PULSE ─────────────── */
void LED_PulseBell(void)
{
    Gpio_WritePin(LED_STATUS_PORT, LED_BELL_PIN, HIGH);
    bell_pulse_start = System_GetTick();
    bell_active      = 1;
}

void LED_BellTick(void)
{
    if (bell_active) {
        if ((System_GetTick() - bell_pulse_start) >= BELL_PULSE_MS) {
            Gpio_WritePin(LED_STATUS_PORT, LED_BELL_PIN, LOW);
            bell_active = 0;
        }
    }
}