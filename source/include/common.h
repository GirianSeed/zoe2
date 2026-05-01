#ifndef __ZOE2_COMMON_H__
#define __ZOE2_COMMON_H__

#include <stddef.h>     // for NULL, size_t
#include <limits.h>     // for MIN/MAX
#include <sys/types.h>
#include <eetypes.h>    // for u_long128
#include "gcctest.h"

// NOTE: ee-gcc 2.9/2.96's limits.h wrongly defines the minimum and maximum
// values of 'long int' as if it were 32-bits wide, when it's actually 64-bits.
// This problem was fixed with ee-gcc 3.2.

/* MSVC defines _countof as an extension to stdlib.h */
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
/* alternate names without the leading underscore */
#define countof(array)  _countof(array)
#define COUNTOF(array)  _countof(array)

/*---------------------------------------------------------------------------*/
/* Common Macro #defines                                                     */
/*---------------------------------------------------------------------------*/

#ifndef MIN
#define MIN(x, y)       (((x) < (y)) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y)       (((x) > (y)) ? (x) : (y))
#endif
#ifndef ABS
#define ABS(x)          (((x) < 0) ? -(x) : (x))
#endif

#define SIGN(x)         (((x) > 0) - ((x) < 0))
//#define SIGN(x)       (((x) < 0) ? -1 : ((x) > 0) ? 1 : 0)
//#define SIGN(x)       (((x) < 0) ? -1 : !!(x))

#ifndef CLAMP
#define CLAMP(x, min, max) (MAX(MIN(x, max), min))
//#define CLAMP(x, min, max) (((x) < (min)) ? (min) : ((x) > (max)) ? (max) : (x))
//#define CLAMP(x, min, max) (((x) > (max)) ? (max) : ((x) < (min)) ? (min) : (x))
#endif

#ifndef SWAP
#define SWAP(a, b)                                              \
    do {                                                        \
        __typeof__(a) _temp = (a);                              \
        (a) = (b); (b) = _temp;                                 \
    } while (0)
#endif

/* Macros for counting and rounding. */
#ifndef HOWMANY
#define HOWMANY(x,y)    (((x) + ((y) - 1)) / (y))
#endif
#ifndef POWEROF2
#define POWEROF2(x)     ((((x) - 1) & (x)) == 0)
#endif
/* rounds to any y */
#ifndef ROUNDUP
#define ROUNDUP(x,y)    ((((x) + ((y) - 1)) / (y)) * (y))
#endif
#ifndef ROUNDDOWN
#define ROUNDDOWN(x,y)  (((x) / (y)) * (y))
#endif
/* if y is a power of two */
#ifndef ROUNDUP2
#define ROUNDUP2(x,y)   (((x) + ((y) - 1)) & ~((y) - 1))
#endif
#ifndef ROUNDDOWN2
#define ROUNDDOWN2(x,y) ((x) & ~((y) - 1))
#endif

/*---------------------------------------------------------------------------*/
/* Compiler attribute #defines                                               */
/*---------------------------------------------------------------------------*/

#if defined(__GNUC__)
#define ALIGN(_x)       __attribute__((aligned(_x)))
#define PACKED          __attribute__((packed))
#elif defined(_MSC_VER)
#define ALIGN(_x)       __declspec(align(_x))
#define PACKED          /* discard */
#else
#define ALIGN(_x)       /* discard */
#define PACKED          /* discard */
#endif
/* common alignments */
#define ALIGN8          ALIGN(8)
#define ALIGN16         ALIGN(16)
#define ALIGN64         ALIGN(64)
#define ALIGN128        ALIGN(128)

// You can also just use ((void)foo) to suppress warnings.
#if defined(__GNUC__)
#define UNUSED          __attribute__((unused))
#else
#define UNUSED          /* discard */
#endif

/*---------------------------------------------------------------------------*/
/* Custom assertion #defines                                                 */
/*---------------------------------------------------------------------------*/

// This will crash the program with the intention of invoking
// the MTS exception handler screen (which was compiled out).
//
// Choosing to dereference 1 instead of 0 distinguishes exceptions raised
// intentionally from actual NULL-pointer dereference bugs, and doubles as
// an unaligned write to memory.
//
#define HANGUP()        (*(int *)1 = 0)

// TODO: Should these be wrapped with 'do {} while (0)'?
#define ASSERT(cond)                                            \
    if (!(cond)) {                                              \
        printf("assertion failed : ");                          \
        printf("\n in %s(%d) from %s \n", __FILE__, __LINE__, __FILE__); \
        HANGUP();                                               \
    }

#define XASSERT(cond, mesg...)                                  \
    if (!(cond)) {                                              \
        printf("assertion failed : " mesg);                     \
        printf("\n in %s(%d) from %s \n", __FILE__, __LINE__, __FILE__); \
        HANGUP();                                               \
    }

/*---------------------------------------------------------------------------*/
/* Color Format #defines                                                     */
/*---------------------------------------------------------------------------*/

/*----- RGBA8888 format -----*/

#ifdef WORDS_BIGENDIAN
#define RGBA_R_SHIFT    (24)
#define RGBA_G_SHIFT    (16)
#define RGBA_B_SHIFT    ( 8)
#define RGBA_A_SHIFT    ( 0)
#else
#define RGBA_R_SHIFT    ( 0)
#define RGBA_G_SHIFT    ( 8)
#define RGBA_B_SHIFT    (16)
#define RGBA_A_SHIFT    (24)
#endif

#define RGBA_R_MASK     (0xff << RGBA_R_SHIFT)
#define RGBA_G_MASK     (0xff << RGBA_G_SHIFT)
#define RGBA_B_MASK     (0xff << RGBA_B_SHIFT)
#define RGBA_A_MASK     (0xff << RGBA_A_SHIFT)

// #define MAKE_RGB_WITHOUT_BITMASK
#ifdef MAKE_RGB_WITHOUT_BITMASK
/* simple version without setting the alpha channel */
#define MAKE_RGB(_r, _g, _b)                                    \
        ((unsigned int)(((_r) << RGBA_R_SHIFT) |                \
                        ((_g) << RGBA_G_SHIFT) |                \
                        ((_b) << RGBA_B_SHIFT)))
#else
#define MAKE_RGB(_r, _g, _b)                                    \
        ((unsigned int)((((_r) & 0xff) << RGBA_R_SHIFT) |       \
                        (((_g) & 0xff) << RGBA_G_SHIFT) |       \
                        (((_b) & 0xff) << RGBA_B_SHIFT)))
#endif

// #define MAKE_RGBA_WITHOUT_BITMASK
#ifdef MAKE_RGBA_WITHOUT_BITMASK
#define MAKE_RGBA(_r,_g,_b,_a)                                  \
        ((unsigned int)(((_r) << RGBA_R_SHIFT) |                \
                        ((_g) << RGBA_G_SHIFT) |                \
                        ((_b) << RGBA_B_SHIFT) |                \
                        ((_a) << RGBA_A_SHIFT)))
#else
#define MAKE_RGBA(_r,_g,_b,_a)                                  \
        ((unsigned int)((((_r) & 0xff) << RGBA_R_SHIFT) |       \
                        (((_g) & 0xff) << RGBA_G_SHIFT) |       \
                        (((_b) & 0xff) << RGBA_B_SHIFT) |       \
                        (((_a) & 0xff) << RGBA_A_SHIFT)))
#endif

#define MAKE_RGB0(_r,_g,_b)     MAKE_RGBA(_r,_g,_b,0x00)
#define MAKE_RGBX(_r,_g,_b)     MAKE_RGBA(_r,_g,_b,0xff) /* full-alpha */
#define MAKE_RGBH(_r,_g,_b)     MAKE_RGBA(_r,_g,_b,0x80) /* half-alpha */

#define GET_R_FROM_RGBA(_rgba)  (((_rgba) >> RGBA_R_SHIFT) & 0xff)
#define GET_G_FROM_RGBA(_rgba)  (((_rgba) >> RGBA_G_SHIFT) & 0xff)
#define GET_B_FROM_RGBA(_rgba)  (((_rgba) >> RGBA_B_SHIFT) & 0xff)
#define GET_A_FROM_RGBA(_rgba)  (((_rgba) >> RGBA_A_SHIFT) & 0xff)

/*----- ARGB8888 format -----*/

#ifdef WORDS_BIGENDIAN
#define ARGB_A_SHIFT    (24)
#define ARGB_R_SHIFT    (16)
#define ARGB_G_SHIFT    ( 8)
#define ARGB_B_SHIFT    ( 0)
#else
#define ARGB_A_SHIFT    ( 0)
#define ARGB_R_SHIFT    ( 8)
#define ARGB_G_SHIFT    (16)
#define ARGB_B_SHIFT    (24)
#endif

#define ARGB_A_MASK     (0xff << ARGB_A_SHIFT)
#define ARGB_R_MASK     (0xff << ARGB_R_SHIFT)
#define ARGB_G_MASK     (0xff << ARGB_G_SHIFT)
#define ARGB_B_MASK     (0xff << ARGB_B_SHIFT)

// #define MAKE_ARGB_WITHOUT_BITMASK
#ifdef MAKE_ARGB_WITHOUT_BITMASK
#define MAKE_ARGB(_a,_r,_g,_b)                                  \
        ((unsigned int)(((_a) << ARGB_A_SHIFT) |                \
                        ((_r) << ARGB_R_SHIFT) |                \
                        ((_g) << ARGB_G_SHIFT) |                \
                        ((_b) << ARGB_B_SHIFT)))
#else
#define MAKE_ARGB(_a,_r,_g,_b)                                  \
        ((unsigned int)((((_a) & 0xff) << ARGB_A_SHIFT) |       \
                        (((_r) & 0xff) << ARGB_R_SHIFT) |       \
                        (((_g) & 0xff) << ARGB_G_SHIFT) |       \
                        (((_b) & 0xff) << ARGB_B_SHIFT)))
#endif

#define MAKE_0RGB(_r,_g,_b)     MAKE_ARGB(0x00,_r,_g,_b)
#define MAKE_XRGB(_r,_g,_b)     MAKE_ARGB(0xff,_r,_g,_b) /* full-alpha */
#define MAKE_HRGB(_r,_g,_b)     MAKE_ARGB(0x80,_r,_g,_b) /* half-alpha */

#define GET_A_FROM_ARGB(_argb)  (((_argb) >> ARGB_A_SHIFT) & 0xff)
#define GET_R_FROM_ARGB(_argb)  (((_argb) >> ARGB_R_SHIFT) & 0xff)
#define GET_G_FROM_ARGB(_argb)  (((_argb) >> ARGB_G_SHIFT) & 0xff)
#define GET_B_FROM_ARGB(_argb)  (((_argb) >> ARGB_B_SHIFT) & 0xff)

/*----- Common Colors -----*/

#define COLOR_BLACK     MAKE_RGB0(  0,  0,  0)
#define COLOR_WHITE     MAKE_RGB0(255,255,255)
#define COLOR_GRAY      MAKE_RGB0(128,128,128)
#define COLOR_RED       MAKE_RGB0(255,  0,  0)
#define COLOR_GREEN     MAKE_RGB0(  0,255,  0)
#define COLOR_BLUE      MAKE_RGB0(  0,  0,255)
#define COLOR_CYAN      MAKE_RGB0(  0,255,255)
#define COLOR_MAGENTA   MAKE_RGB0(255,  0,255)
#define COLOR_YELLOW    MAKE_RGB0(255,255,  0)

/*---------------------------------------------------------------------------*/
// NOTE: (bool)0.5 evaluates to true, whereas (int)0.5 evaluates to 0.

typedef int             BOOL;

#ifndef FALSE
#define FALSE           (0)
#endif
#ifndef TRUE
#define TRUE            (!FALSE)
#endif

/*---------------------------------------------------------------------------*/

typedef signed char     sint8;          //  8-bit signed integer
typedef signed short    sint16;         // 16-bit signed integer
typedef signed int      sint32;         // 32-bit signed integer
typedef signed long     sint64;         // 64-bit signed integer

typedef unsigned char   uint8;          //  8-bit unsigned integer
typedef unsigned short  uint16;         // 16-bit unsigned integer
typedef unsigned int    uint32;         // 32-bit unsigned integer
typedef unsigned long   uint64;         // 64-bit unsigned integer

// https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
// https://gcc.gnu.org/onlinedocs/gccint/Machine-Modes.html

typedef long128         sint128;        // 128-bit signed integer
typedef u_long128       uint128;        // 128-bit unsigned integer

/*---------------------------------------------------------------------------*/

typedef union DAT64 {
    uint64  u64;        // 64-bit unsigned integer
    uint32  u32[2];     // 32-bit unsigned integer
    uint16  u16[4];     // 16-bit unsigned integer
    uint8   u8[8];      //  8-bit unsigned integer
    float   f[2];       // 32-bit floating point
} ALIGN8 DAT64;

typedef union DAT128 {
    uint128 u128;       // 128-bit unsigned integer
    uint64  u64[2];     //  64-bit unsigned integer
    uint32  u32[4];     //  32-bit unsigned integer
    uint16  u16[8];     //  16-bit unsigned integer
    uint8   u8[16];     //   8-bit unsigned integer
    float   f[4];       //  32-bit floating point
} ALIGN16 DAT128;

typedef struct FVECTOR {
    float vx;
    float vy;
    float vz;
    float vw;
} ALIGN16 FVECTOR;

typedef struct FMATRIX {
    float m[4][4];
} ALIGN16 FMATRIX;

#endif // {{{ END OF FILE }}}
