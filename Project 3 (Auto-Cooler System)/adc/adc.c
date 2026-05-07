/*
 * RM0090 register steps:
 *   §13.3.1  Clock prescaler (ADC_CCR ADCPRE)
 *   §13.3.4  Continuous mode (ADC_CR2 CONT)
 *   §13.3.8  EOC interrupt   (ADC_CR1 EOCIE)
 *   §13.8.5  Sample time     (ADC_SMPR2)
 *   §13.8.9  Sequence config (ADC_SQR1, ADC_SQR3)
 */

#include "adc.h"
#include "adc_private.h"
#include "gpio.h"
#include "nvic.h"

/* Registered callback — stored and called from the ISR */
static AdcCallback_t adc_callback = 0;

/* ================================================================== */
/* Adc_Init                                                             */
/* Configures ADC1 for continuous, interrupt-driven operation.        */
/* PA1 must already be in Analog mode (call Gpio_Init before this).   */
/* ================================================================== */
void Adc_Init(AdcCallback_t callback)
{
    /* Store the user callback */
    adc_callback = callback;

    /* -- 1. PA1 → Analog mode (connects pin to ADC, disables digital) */
    Gpio_Init(GPIO_A, 1, GPIO_ANALOG, GPIO_NO_PULL);

    /* -- 2. ADC Common: prescaler = PCLK2 / 4 ----------------------- */
    /* CCR bits [17:16] = 01 → divide by 4                             */
    /* At 16 MHz PCLK2:  16 / 4 = 4 MHz ADC clock (max 36 MHz per RM) */
    ADC_COMMON->CCR &= ~(3UL << 16);
    ADC_COMMON->CCR |=  ADC_CCR_ADCPRE_DIV4;

    /* -- 3. ADC1 CR1: 12-bit resolution (bits[25:24]=00, default),   */
    /*    EOC interrupt enable (EOCIE bit 5)                           */
    ADC1->CR1  = 0;
    ADC1->CR1 |= ADC_CR1_EOCIE;

    /* -- 4. ADC1 CR2: single conversion mode (CONT bit 0, default) --- */
    ADC1->CR2  = 0;
    /* SWSTART is triggered manually in main.c and Adc_Init */

    /* -- 5. Sample time for channel 1: 480 cycles (most accurate)    */
    /* SMPR2 bits [5:3] = 111 (channel 1 sample time field)           */
    ADC1->SMPR2 &= ~(7UL << 3);
    ADC1->SMPR2 |=  (7UL << 3);    /* 111 = 480 cycles */

    /* -- 6. Regular sequence: 1 conversion, channel 1 first ---------- */
    ADC1->SQR1 = 0;                 /* L[3:0] = 0000 → 1 conversion   */
    ADC1->SQR3 = (1UL << 0);       /* SQ1    = channel 1              */

    /* -- 7. Enable ADC1 (ADON bit) ----------------------------------- */
    ADC1->CR2 |= ADC_CR2_ADON;
    /* Wait for ADC stabilisation (Tstab ≈ 3 µs, RM0090 §13.3.1)     */
    for (volatile uint32_t i = 0; i < 500; i++);

    /* -- 8. Enable ADC IRQ in NVIC ----------------------------------- */
    Nvic_EnableIRQ(ADC_IRQ_NUMBER);

    /* -- 9. Start the first conversion -------------------------------- */
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/* ================================================================== */
/* Adc_StartConversion                                                  */
/* Triggers a single or the first of a continuous sequence.           */
/* ================================================================== */
void Adc_StartConversion(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/* ================================================================== */
/* Adc_RawToTemperature                                                 */
/* Converts a 12-bit ADC count to degrees Celsius.                    */
/* Fixed-point formula (avoids floating-point in ISR):                */
/*   Temp = raw × 330 / 4095   (°C, with one decimal digit accuracy)  */
/* ================================================================== */
float Adc_RawToTemperature(uint16_t raw)
{
    /* LM35: 10 mV/°C. 
     * Vref = 3.3 V, 12-bit ADC (4095 steps)
     * Temp = (raw * 3.3 * 100) / 4095 
     *      = (raw * 330) / 4095                                    */
    return (float)((uint32_t)raw * 330UL) / 4095.0f;
}

/* ================================================================== */
/* ADC_IRQHandler                                                       */
/* Fires on every End-of-Conversion in continuous mode.               */
/* Reads ADC1->DR (clears EOC flag), dispatches to the callback.      */
/* ================================================================== */
void ADC_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_EOC)
    {
        /* Reading DR automatically clears the EOC flag (229) */
        uint16_t raw = (uint16_t)(ADC1->DR & 0x0FFFUL);

        /* Dispatch to registered callback */
        if (adc_callback != 0)
        {
            adc_callback(raw);
        }
    }
}
