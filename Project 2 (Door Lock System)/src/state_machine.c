#include "state_machine.h"
#include "display.h"
#include "main.h"      /* To get LOCKOUT_THRESHOLD */
#include "exti.h"      /* To get Exti_Disable/Enable for critical sections */
#include <stdint.h>

/* ─────────────── PRIVATE STATE ─────────────── */
/* Must be volatile because it's a Shared Resource */
static volatile SystemState_t current_state  = STATE_LOCKED;
static volatile uint8_t       failure_count  = 0;
static volatile uint8_t       progress       = 0;

/* ─────────────── INIT ─────────────── */
void SM_Init(void)
{
    /* No interrupts during init */
    current_state = STATE_LOCKED;
    failure_count = 0;
    progress      = 0;

    SevenSeg_ShowDigit(0);
    LEDBar_SetProgress(0);
    LED_SetSuccess(0);
    LED_SetAlarm(0);
}

/* ─────────────── MEALY TRANSITIONS + OUTPUTS ─────────────── */
void SM_ProcessEvent(SystemEvent_t event)
{
    /* 
     * We must disable the EXTI interrupts briefly so that a button press so doesn't interrupt a keypad event processing. */
    Exti_Disable(BTN_RESET_PIN);
    Exti_Disable(BTN_BELL_PIN);
    Exti_Disable(BTN_LOCK_PIN);

    switch (current_state)
    {
        case STATE_LOCKED:
            switch (event)
            {
                case EVENT_KEY_PRESSED:
                    progress++;
                    LEDBar_SetProgress(progress);
                    break;

                case EVENT_SEQUENCE_COMPLETE:
                    progress = 0;
                    LEDBar_SetProgress(0);
                    LED_SetSuccess(1);
                    current_state = STATE_UNLOCKED;
                    break;

                case EVENT_INVALID_KEY:
                    progress = 0;
                    failure_count++;
                    LEDBar_SetProgress(0);
                    SevenSeg_ShowDigit(failure_count);

                    if (failure_count >= LOCKOUT_THRESHOLD)
                    {
                        LED_SetAlarm(1);
                        current_state = STATE_ALARM;
                    }
                    break;

                case EVENT_DOORBELL:
                    LED_PulseBell();
                    break;

                case EVENT_EMERGENCY_RESET:
                    progress = 0;
                    failure_count = 0;
                    LEDBar_SetProgress(0);
                    LED_SetAlarm(0);
                    SevenSeg_ShowDigit(0);
                    break;

                default: break;
            }
            break;

        case STATE_UNLOCKED:
            switch (event)
            {
                case EVENT_LOCK_CMD:
                    LED_SetSuccess(0);
                    current_state = STATE_LOCKED;
                    break;

                case EVENT_DOORBELL:
                    LED_PulseBell();
                    break;

                default: break;
            }
            break;

        case STATE_ALARM:
            switch (event)
            {
                case EVENT_EMERGENCY_RESET:
                    LED_SetAlarm(0);
                    LEDBar_SetProgress(0);
                    failure_count = 0;
                    progress = 0;
                    SevenSeg_ShowDigit(0);
                    current_state = STATE_LOCKED;
                    break;
                
                case EVENT_DOORBELL: /* Doorbell should work in Alarm too */
                    LED_PulseBell();
                    break;

                default: break;
            }
            break;

        default:
            SM_Init();
            break;
    }

    /* RE-ENABLE INTERRUPTS after the state transition is complete */
    Exti_Enable(BTN_RESET_PIN);
    Exti_Enable(BTN_BELL_PIN);
    Exti_Enable(BTN_LOCK_PIN);
}

/* ─────────────── GETTER ─────────────── */
SystemState_t SM_GetState(void)
{
    /* Minimal Critical Section for reading shared data. */
    SystemState_t temp;
    Exti_Disable(BTN_LOCK_PIN); 
    temp = current_state;
    Exti_Enable(BTN_LOCK_PIN);
    return temp;
}