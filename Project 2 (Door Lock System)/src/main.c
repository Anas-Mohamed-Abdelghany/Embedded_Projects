#include "main.h"
#include "rcc.h"          /* Manual clock driver */
#include "gpio.h"         /* Manual GPIO driver */
#include "display.h"      /* Your Display logic */
#include "keypad.h"       /* Your Keypad logic */
#include "exti.h"         /* Your EXTI driver */
#include "state_machine.h"

/* ─────────────── BARE-METAL TIMEBASE ─────────────── */
volatile uint32_t msTicks = 0; // Shared resource

void SysTick_Handler(void) {
    msTicks++;
}

uint32_t System_GetTick(void) {
    return msTicks;
}

void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

/* ─────────────── ENTRY POINT ─────────────── */
int main(void)
{
    /* 1. Init System Clocks */
    Rcc_Init(); 

    /* 2. Configure SysTick for 1ms interrupts 
     * (Assuming 16MHz default clock. 16000 ticks = 1ms) 
     */
    *((volatile uint32_t *)0xE000E014) = 16000 - 1; // SysTick Reload Value
    *((volatile uint32_t *)0xE000E018) = 0;         // Current Value
    *((volatile uint32_t *)0xE000E010) = 0x07;      // Enable, Interrupt, Source=Processor

    /* 3. Initialise Peripheral Drivers */
    Display_Init();
    Keypad_Init();
    Interrupts_Init(); // This calls Exti_EnableInterrupt internally

    /* 4. Initialise State Machine */
    SM_Init();

    /* ── Super-loop ── */
    while (1)
    {
        /* 1. Poll Keypad (Checks keys, handles debounce, sends SM events) */
        Keypad_Update();

        /* 2. Update Timers (Handles doorbell LED timing) */
        LED_BellTick();

        /* 3. Small delay for system stability */
        delay_ms(10);
    }
}