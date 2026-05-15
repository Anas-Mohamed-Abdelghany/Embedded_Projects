/**
 * @file main.h
 * @brief Main entry point for STM32 dual-elevator system.
 */

#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>


/* --- Bit Manipulation Macros --- */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (BIT))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))

/* --- Critical Section Macros --- */
#define __disable_irq() __asm volatile ("cpsid i" : : : "memory")
#define __enable_irq()  __asm volatile ("cpsie i" : : : "memory")

#define Enter_Critical()    __disable_irq()
#define Exit_Critical()     __enable_irq()

/* Boolean Types */
typedef enum {
    FALSE = 0,
    TRUE  = 1
} bool_t;

/* Common Return Codes */
typedef enum {
    OK = 0,
    ERROR,
    BUSY,
    TIMEOUT
} status_t;

/* 
 * MCU Role selection is now handled by CMake.
 * The build system will pass -DMASTER_MCU or -DSLAVE_MCU.
 */

#if defined(MASTER_MCU) && defined(SLAVE_MCU)
#error "Cannot define both MASTER_MCU and SLAVE_MCU simultaneously!"
#endif

#if !defined(MASTER_MCU) && !defined(SLAVE_MCU)
#error "One MCU role must be defined (MASTER_MCU or SLAVE_MCU)!"
#endif

#endif /* MAIN_H */
