#ifndef LCD_PRIVATE_H
#define LCD_PRIVATE_H

#include <stdint.h>

/* ---- LCD GPIO pin assignments on GPIOD ---- */
#define LCD_PORT   GPIO_D
#define LCD_RS     0     /* PD0 - Register Select */
#define LCD_EN     1     /* PD1 - Enable          */
#define LCD_D4     4     /* PD4 - Data bit 4      */
#define LCD_D5     5     /* PD5 - Data bit 5      */
#define LCD_D6     6     /* PD6 - Data bit 6      */
#define LCD_D7     7     /* PD7 - Data bit 7      */

/* ---- HD44780 DDRAM row start addresses ---- */
#define LCD_ROW0_ADDR   0x00U
#define LCD_ROW1_ADDR   0x40U

/* ---- HD44780 command bytes ---- */
#define LCD_CMD_CLEAR         0x01U   /* Clear display, cursor home     */
#define LCD_CMD_ENTRY_MODE    0x06U   /* Cursor increment, no shift     */
#define LCD_CMD_DISPLAY_ON    0x0CU   /* Display ON, cursor OFF         */
#define LCD_CMD_FUNCTION_SET  0x28U   /* 4-bit, 2 lines, 5×8 font      */
#define LCD_CMD_DISPLAY_OFF   0x08U   /* Display OFF                    */
#define LCD_CMD_SET_DDRAM     0x80U   /* OR with DDRAM address to set   */

/* send_nibble — writes 4 bits to D4-D7 and pulses EN. */
static void send_nibble(uint8_t nibble);

/* send_byte — sends a full 8-bit value as two nibbles. */
static void send_byte(uint8_t byte, uint8_t rs);


#endif /* LCD_PRIVATE_H */
