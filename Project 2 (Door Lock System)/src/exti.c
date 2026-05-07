#include "exti.h"        /* Use the header we defined earlier */
#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "state_machine.h"

/* ─────────────── REGISTER OVERLAYS ─────────────── */
typedef struct {
    volatile uint32_t IMR;   /* 0x00 */
    volatile uint32_t EMR;   /* 0x04 - Event mask Reg. -> 0 = masked , 1 = unmasked */
    volatile uint32_t RTSR;  /* 0x08 */
    volatile uint32_t FTSR;  /* 0x0C */
    volatile uint32_t SWIER; /* 0x10 */
    volatile uint32_t PR;    /* 0x14 - Pending Reg. -> 0 = no trigger , 1 = trigger request*/
} ExtiType;

typedef struct {
    volatile uint32_t MEMRMP;    /* 0x00 - Memory remap register */
    volatile uint32_t PMC;       /* 0x04 - Peripheral mode configuration */
    volatile uint32_t EXTICR[4]; /* 0x08 - External interrupt configuration registers */
} SyscfgType;

#define EXTI   ((ExtiType *)   0x40013C00UL)
#define SYSCFG ((SyscfgType *) 0x40013800UL)

/* NVIC Enable Register */
#define NVIC_ISER ((volatile uint32_t *) 0xE000E100UL)

/* ─────────────── CALLBACK STORAGE ─────────────── */
static ExtiCallback_t extiCallbacks[16] = {0};

/* ─────────────── WRAPPERS FOR STATE MACHINE ─────────────── */
/* These small functions are passed as callbacks to the EXTI driver */
void Handle_Reset_Button(void) { SM_ProcessEvent(EVENT_EMERGENCY_RESET); }
void Handle_Bell_Button(void)  { SM_ProcessEvent(EVENT_DOORBELL); }
void Handle_Lock_Button(void)  { SM_ProcessEvent(EVENT_LOCK_CMD); }

/* ─────────────── INIT ─────────────── */
void Interrupts_Init(void)
{
    /* 1. Enable Clocks */
    Rcc_Enable('E'); 
    // You also need to enable the SYSCFG clock (RCC_APB2ENR bit 14)
    *((volatile uint32_t *)(0x40023800 + 0x44)) |= (1 << 14); 

    /* 2. Configure GPIO Pins as Input */
    Gpio_Init(BTN_PORT, BTN_RESET_PIN, GPIO_INPUT, HIGH);
    Gpio_Init(BTN_PORT, BTN_BELL_PIN,  GPIO_INPUT, HIGH);
    Gpio_Init(BTN_PORT, BTN_LOCK_PIN,  GPIO_INPUT, HIGH);

    /* 3. Use API to link buttons to SM events */
    Exti_EnableInterrupt(BTN_RESET_PIN, BTN_PORT, FALLING, Handle_Reset_Button);
    Exti_EnableInterrupt(BTN_BELL_PIN,  BTN_PORT, FALLING, Handle_Bell_Button);
    Exti_EnableInterrupt(BTN_LOCK_PIN,  BTN_PORT, FALLING, Handle_Lock_Button);
}

/* ─────────────── DRIVER IMPLEMENTATION ─────────────── */
void Exti_EnableInterrupt(uint8_t lineNumber, char portName, ExtiEdge_t edge, ExtiCallback_t callback)
{
    /* 1. Store callback */
    extiCallbacks[lineNumber] = callback;

    /* 2. SYSCFG: Route Port to EXTI Line */
    uint8_t index = lineNumber / 4;
    uint8_t shift = (lineNumber % 4) * 4;
    uint32_t portVal = portName - 'A';
    SYSCFG->EXTICR[index] &= ~(0xF << shift);
    SYSCFG->EXTICR[index] |= (portVal << shift);

    /* 3. Edge Selection */
    EXTI->RTSR &= ~(1 << lineNumber);
    EXTI->FTSR &= ~(1 << lineNumber);
    if (edge == RISING || edge == BOTH) EXTI->RTSR |= (1 << lineNumber);
    if (edge == FALLING || edge == BOTH) EXTI->FTSR |= (1 << lineNumber);

    /* 4. Unmask in EXTI */
    EXTI->IMR |= (1 << lineNumber);

    /* 5. Enable in NVIC (mapping: lines 0-4 = IRQ 6-10) */
    uint8_t irqNum = lineNumber + 6; 
    NVIC_ISER[irqNum / 32] |= (1 << (irqNum % 32));
}

void Exti_Disable(uint8_t lineNumber) {
    EXTI->IMR &= ~(1 << lineNumber);
}

void Exti_Enable(uint8_t lineNumber) {
    EXTI->IMR |= (1 << lineNumber);
}

/* ─────────────── ISR HANDLERS ─────────────── */

void EXTI0_IRQHandler(void)
{
    /* Check if Line 0 fired */
    if (EXTI->PR & (1 << 0)) {
        EXTI->PR |= (1 << 0);           /* CLEAR PENDING FLAG: WRITE 1 */
        if (extiCallbacks[0]) extiCallbacks[0](); /* Execute Callback */
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & (1 << 1)) {
        EXTI->PR |= (1 << 1);
        if (extiCallbacks[1]) extiCallbacks[1]();
    }
}

void EXTI2_IRQHandler(void)
{
    if (EXTI->PR & (1 << 2)) {
        EXTI->PR |= (1 << 2);
        if (extiCallbacks[2]) extiCallbacks[2]();
    }
}
