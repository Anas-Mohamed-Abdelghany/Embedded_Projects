#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

/* Returns ASCII char of pressed key, or 0 if no key pressed */
char     Keypad_Scan(void);
void     Keypad_Init(void);

/* Process keypad and push events into the state machine */
void     Keypad_Update(void);

#endif /* KEYPAD_H */
