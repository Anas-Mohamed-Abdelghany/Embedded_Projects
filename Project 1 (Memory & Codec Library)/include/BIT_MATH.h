#ifndef BIT_MATH_H
#define BIT_MATH_H

/* Set a single bit at position BIT in register REG */
#define SET_BIT(REG, BIT)       ((REG) |=  (1u << (BIT)))

/* Clear a single bit at position BIT in register REG */
#define CLEAR_BIT(REG, BIT)     ((REG) &= ~(1u << (BIT)))

/* Toggle a single bit at position BIT in register REG */
#define TOGGLE_BIT(REG, BIT)    ((REG) ^=  (1u << (BIT)))

/* Read a single bit at position BIT in register REG (returns 0 or non-zero) */
#define READ_BIT(REG, BIT)      (((REG) >> (BIT)) & 1u)

/* Set multiple bits using a mask */
#define SET_MASK(REG, MASK)     ((REG) |=  (MASK))

/* Clear multiple bits using a mask */
#define CLEAR_MASK(REG, MASK)   ((REG) &= ~(MASK))

/* Write VALUE into a bit-field of REG.
 * START: least-significant bit position of the field.
 * WIDTH: number of bits in the field. */
#define WRITE_FIELD(REG, START, WIDTH, VALUE) \
    ((REG) = (((REG) & ~(((1u << (WIDTH)) - 1u) << (START))) | \
              (((u32)(VALUE) & ((1u << (WIDTH)) - 1u)) << (START))))

/* Read/extract a bit-field value from REG.
 * START: least-significant bit position of the field.
 * WIDTH: number of bits in the field. */
#define READ_FIELD(REG, START, WIDTH) \
    (((REG) >> (START)) & ((1u << (WIDTH)) - 1u))

#endif
