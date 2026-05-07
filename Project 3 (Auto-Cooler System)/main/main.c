#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"
#include "lcd.h"
#include "state_machine.h"
#include "nvic.h"

/* Include the private hardware definitions */
#include "main_private.h"

/* Configuration Constants */
#define LOOP_DELAY_MS  100

static volatile uint16_t adc_raw   = 0;
static volatile uint8_t  adc_ready = 0;

/* ================================================================== */
/* main                                                               */
/* ================================================================== */
int main(void)
{
    /* ---- 1. Hardware/System Initialization ------------------------ */
    
    /* Enable FPU (Full access to CP10 and CP11) */
    SCB_CPACR |= ((3UL << 10*2) | (3UL << 11*2));

    Rcc_Init();

    /* ---- 2. Enable Peripheral Clocks ------------------------------ */
    Rcc_Enable(RCC_GPIOA);   /* PA1  -> ADC1_IN1    */
    Rcc_Enable(RCC_GPIOB);   /* PB4  -> TIM3_CH1    */
    Rcc_Enable(RCC_GPIOC);   /* PC0  -> Alarm LED   */
    Rcc_Enable(RCC_GPIOD);   /* PDxx -> LCD         */
    Rcc_Enable(RCC_TIM2);    /* TIM2 -> System Tick */
    Rcc_Enable(RCC_TIM3);    
    Rcc_Enable(RCC_ADC1);    

    /* ---- 3. Driver Initialization --------------------------------- */
    Timer_Init();            
    Pwm_Init();                
    LCD_Init();                
    FSM_Init();                
    Adc_Init(on_adc_reading);  

    /* ---- 4. Main Control Loop ------------------------------------- */
    while (1)
    {
        /* Trigger a new ADC conversion */
        Adc_StartConversion(); 

        if (adc_ready)
        {
            /* --- CRITICAL SECTION START --- */
            /* Disable ADC Interrupt (IRQ 18) to prevent a "torn read" */
            Nvic_DisableIRQ(18); 
            uint16_t raw = adc_raw;
            adc_ready = 0;
            Nvic_EnableIRQ(18); 
            /* --- CRITICAL SECTION END --- */

            /* Convert raw bits to Celsius */
            float temperature = Adc_RawToTemperature(raw);
            
            /* Logic processing, LCD update, and Motor control */
            FSM_Update(temperature);
        }

        /* Wait before next sample */
        delay_ms(LOOP_DELAY_MS); 
    }

    return 0;
}

/* ================================================================== */
/* Private Function Implementations                                   */
/* ================================================================== */

/* ADC callback called from the Interrupt Service Routine */
static void on_adc_reading(uint16_t raw)
{
    adc_raw   = raw;
    adc_ready = 1;
}
