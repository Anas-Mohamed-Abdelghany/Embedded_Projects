/*
 * Hardware wiring (all on GPIOD):
 *   PD0 → RS   (Register Select: LOW=command, HIGH=data)
 *   PD1 → EN   (Enable pulse)
 *   PD4 → D4
 *   PD5 → D5
 *   PD6 → D6
 *   PD7 → D7
 *   RW  → GND  (write-only, always LOW)
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *str);

#endif /* LCD_H */
