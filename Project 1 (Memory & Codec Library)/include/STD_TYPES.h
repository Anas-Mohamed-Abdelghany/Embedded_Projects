#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

/* Fixed-width unsigned integer type aliases */
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

/* Fixed-width signed integer type aliases */
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;

/* Boolean type */
typedef uint8_t   bool_t;
#define TRUE      ((bool_t)1u)
#define FALSE     ((bool_t)0u)

/* Standard function return type */
typedef uint8_t   Std_ReturnType;
#define E_OK      ((Std_ReturnType)0u)
#define E_NOT_OK  ((Std_ReturnType)1u)

/* Generic pointer type for raw memory access */
typedef void*     vptr_t;

#endif /* STD_TYPES_H */

