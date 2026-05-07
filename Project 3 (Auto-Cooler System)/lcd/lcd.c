/*
 * All pin control goes through Gpio_WritePin() (our GPIO driver).
 * Timing values from the HD44780 datasheet.
 *
 * 4-bit mode: every byte is sent as two consecutive 4-bit nibbles.
 * High nibble is sent first, then low nibble.
 *
 * Cursor reposition (SetCursor) is used instead of LCD_Clear on every
 * update to prevent visible flicker — writing 16 characters overwrites
 * old content without any blank period.
 */

#include "lcd.h"            // Public API
#include "lcd_private.h"    // Internal definitions and static prototypes
#include "gpio.h"
#include "timer.h"

/* send_nibble — writes 4 bits to D4-D7 and pulses EN.
 * EN must be HIGH for ≥450 ns then LOW for ≥270 ns (HD44780 tEH/tEL).
 * delay_us(1) at 16 MHz satisfies both requirements. */
static void send_nibble(uint8_t nibble)
{
    // 1. Apply data to pins
    Gpio_WritePin(LCD_PORT, LCD_D4, (nibble >> 0) & 0x01U);
    Gpio_WritePin(LCD_PORT, LCD_D5, (nibble >> 1) & 0x01U);
    Gpio_WritePin(LCD_PORT, LCD_D6, (nibble >> 2) & 0x01U);
    Gpio_WritePin(LCD_PORT, LCD_D7, (nibble >> 3) & 0x01U);

    // 2. Small delay to let pins settle in simulation
    delay_us(10); 

    // 3. Pulse Enable HIGH
    Gpio_WritePin(LCD_PORT, LCD_EN, HIGH);
    delay_us(10); /* Sufficient for Proteus, much faster than 2ms */
    
    // 4. Pulse Enable LOW
    Gpio_WritePin(LCD_PORT, LCD_EN, LOW);
    delay_us(10); 
}

/*
 * send_byte — sends a full 8-bit value as two nibbles.
 * rs = LOW  → command register
 * rs = HIGH → data register (character)
 */
static void send_byte(uint8_t byte, uint8_t rs)
{
    Gpio_WritePin(LCD_PORT, LCD_RS, rs);
    send_nibble(byte >> 4);       /* high nibble first */
    send_nibble(byte & 0x0FU);    /* low  nibble       */
}

/* ================================================================== */
/* Public API                                                           */
/* ================================================================== */

/* Initialises GPIO pins and the HD44780 controller.
 * Follows the "Initialising by Instruction" sequence from the HD44780
 * datasheet (§2.1, p. 46) — mandatory for reliable power-on startup.
 */
void LCD_Init(void)
{
    /* ---- Init all LCD pins as push-pull output ---- */
    Gpio_Init(LCD_PORT, LCD_RS, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_EN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D4, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D5, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D6, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D7, GPIO_OUTPUT, GPIO_PUSH_PULL);

    Gpio_WritePin(LCD_PORT, LCD_RS, LOW);
    Gpio_WritePin(LCD_PORT, LCD_EN, LOW);

    /* ---- 1. Power-on delay (VERY long for simulation) ---- */
    delay_ms(100);

    /* ---- 2. Reset sequence (Force 8-bit mode 3 times) ---- */
    send_nibble(0x03); delay_ms(10);
    send_nibble(0x03); delay_ms(1);
    send_nibble(0x03); delay_ms(1);

    /* ---- 3. Switch to 4-bit mode ---- */
    /* Sending 0x02 sets the LCD to 4-bit operation */
    send_nibble(0x02); delay_ms(1);

    /* ---- 4. Function Set: 4-bit, 2 lines, 5x8 font (0x28) ---- */
    send_byte(LCD_CMD_FUNCTION_SET, LOW); delay_ms(1);

    /* ---- 5. Display Control ---- */
    send_byte(LCD_CMD_DISPLAY_OFF, LOW);  delay_ms(1);
    send_byte(LCD_CMD_CLEAR, LOW);        delay_ms(5);
    send_byte(LCD_CMD_ENTRY_MODE, LOW);   delay_ms(1);
    send_byte(LCD_CMD_DISPLAY_ON, LOW);   delay_ms(1);
}
void LCD_Clear(void)
{
    send_byte(LCD_CMD_CLEAR, LOW);
    delay_ms(2);
}

/* LCD_SetCursor — moves cursor to (row, col) by writing the DDRAM address.
 * row 0 = line 1 (DDRAM offset 0x00)
 * row 1 = line 2 (DDRAM offset 0x40)
 * col 0–15 */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0U) ? LCD_ROW0_ADDR : LCD_ROW1_ADDR;
    addr += col;
    send_byte(LCD_CMD_SET_DDRAM | addr, LOW);
    delay_us(50);
}

/* LCD_WriteChar — writes one ASCII character at the current cursor position.
 * 0xDF in HD44780 CGROM A00 = degree symbol (°). */
void LCD_WriteChar(char c)
{
    send_byte((uint8_t)c, HIGH);   /* RS = HIGH → data register */
    delay_us(50);
}

/* LCD_WriteString — writes a null-terminated string starting at the cursor. */
void LCD_WriteString(const char *str)
{
    while (*str != '\0')
    {
        LCD_WriteChar(*str);
        str++;
    }
}
