/* GPIO driver implementation. */

#include "gpio.h"
#include "gpio_private.h"

/* ================================================================== */
/* Gpio_Init                                                            */
/* Configures PinMode (MODER) and DefaultState (PUPDR or OTYPER).     */
/* ================================================================== */
void Gpio_Init(uint8_t PortName, uint8_t PinNumber,
               uint8_t PinMode,  uint8_t DefaultState)
{
    /* Resolve port name to struct pointer using the address lookup table */
    uint8_t   addrIdx    = PortName - GPIO_A;
    GpioType *gpioDevice = (GpioType *) addressMap[addrIdx];

    /* -- Step 1: Set pin mode in MODER (2-bit field per pin) --------- */
    gpioDevice->MODER &= ~(0x03UL << (PinNumber * 2));     /* clear    */
    gpioDevice->MODER |=  ((uint32_t)PinMode << (PinNumber * 2)); /* set */

    /* -- Step 2: Configure DefaultState depending on mode ------------ */
    if (PinMode == GPIO_INPUT)
    {
        /* Input mode: DefaultState = pull config (PUPDR) */
        gpioDevice->PUPDR &= ~(0x03UL << (PinNumber * 2));
        gpioDevice->PUPDR |=  ((uint32_t)DefaultState << (PinNumber * 2));
    }
    else if (PinMode == GPIO_OUTPUT || PinMode == GPIO_AF)
    {
        /* Output / AF mode: DefaultState = output type (OTYPER, 1 bit) */
        gpioDevice->OTYPER &= ~(0x01UL << PinNumber);
        gpioDevice->OTYPER |=  ((uint32_t)DefaultState << PinNumber);
    }
    else if (PinMode == GPIO_ANALOG)
    {
        /* Analog mode: disable any pull-up/down resistor */
        gpioDevice->PUPDR &= ~(0x03UL << (PinNumber * 2));
    }
}

/* ================================================================== */
/* Gpio_WritePin                                                        */
/* Drives a pin HIGH or LOW via ODR. Returns NOK if pin is input.     */
/* ================================================================== */
uint8_t Gpio_WritePin(uint8_t PortName, uint8_t PinNumber, uint8_t Data)
{
    uint8_t   addrIdx    = PortName - GPIO_A;
    GpioType *gpioDevice = (GpioType *) addressMap[addrIdx];

    /* Validate: refuse to write if pin is in input mode */
    uint8_t currentMode = (uint8_t)
        ((gpioDevice->MODER >> (PinNumber * 2)) & 0x03U);

    if (currentMode == GPIO_INPUT)
    {
        return GPIO_NOK;
    }

    /* Read-modify-write on ODR to avoid touching other pins */
    gpioDevice->ODR &= ~(0x01UL << PinNumber);
    gpioDevice->ODR |=  ((uint32_t)(Data & 0x01U) << PinNumber);

    return GPIO_OK;
}

/* ================================================================== */
/* Gpio_ReadPin                                                         */
/* Returns the current logic level on the pin via IDR.                */
/* ================================================================== */
uint8_t Gpio_ReadPin(uint8_t PortName, uint8_t PinNumber)
{
    uint8_t   addrIdx    = PortName - GPIO_A;
    GpioType *gpioDevice = (GpioType *) addressMap[addrIdx];

    return (uint8_t)((gpioDevice->IDR >> PinNumber) & 0x01U);
}

/* ================================================================== */
/* Gpio_SetAF - Writes the alternate function number into AFR[0] (pins 0-7) */
/* or AFR[1] (pins 8-15). 4 bits per pin.                             */
/* ================================================================== */
void Gpio_SetAF(uint8_t PortName, uint8_t PinNumber, uint8_t AFNumber)
{
    uint8_t   addrIdx    = PortName - GPIO_A;
    GpioType *gpioDevice = (GpioType *) addressMap[addrIdx];

    uint8_t afrIdx = PinNumber / 8;         /* AFR[0] for pins 0-7, AFR[1] for 8-15 */
    uint8_t bitPos = (PinNumber % 8) * 4;   /* 4 bits per pin */

    gpioDevice->AFR[afrIdx] &= ~(0x0FUL << bitPos);
    gpioDevice->AFR[afrIdx] |=  ((uint32_t)AFNumber << bitPos);
}
