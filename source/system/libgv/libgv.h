#ifndef __ZOE2_LIBGV_H__
#define __ZOE2_LIBGV_H__

#include <libpad.h>     // for SCE_PAD*
#include "common.h"

class GV_ACTOR;

/*---------------------------------------------------------------------------*/

enum {
    PAD_U       = SCE_PADLup,     // 0x1000 ↑
    PAD_D       = SCE_PADLdown,   // 0x4000 ↓
    PAD_L       = SCE_PADLleft,   // 0x8000 ←
    PAD_R       = SCE_PADLright,  // 0x2000 →
    PAD_A       = SCE_PADRright,  // 0x0020 ○
    PAD_B       = SCE_PADRdown,   // 0x0040 ×
    PAD_X       = SCE_PADRup,     // 0x0010 △
    PAD_Y       = SCE_PADRleft,   // 0x0080 □
    PAD_L1      = SCE_PADL1,      // 0x0004 L1
    PAD_R1      = SCE_PADR1,      // 0x0008 R1
    PAD_L2      = SCE_PADL2,      // 0x0001 L2
    PAD_R2      = SCE_PADR2,      // 0x0002 R2
    PAD_STA     = SCE_PADstart,   // 0x0800 STA
    PAD_SEL     = SCE_PADselect,  // 0x0100 SEL
    PAD_AL      = SCE_PADi,       // 0x0200 L3
    PAD_AR      = SCE_PADj,       // 0x0400 R3

    /* button masks */
    PAD_UDLR    = (PAD_U  | PAD_D  | PAD_L  | PAD_R),
    PAD_ABXY    = (PAD_A  | PAD_B  | PAD_X  | PAD_Y),
    PAD_LR      = (PAD_L1 | PAD_L2 | PAD_R1 | PAD_R2)
};

/* button aliases */
#define PAD_UP          PAD_U           // ↑
#define PAD_DOWN        PAD_D           // ↓
#define PAD_LEFT        PAD_L           // ←
#define PAD_RIGHT       PAD_R           // →
#define PAD_CIRCLE      PAD_A           // ○
#define PAD_CROSS       PAD_B           // ×
#define PAD_TRIANGLE    PAD_X           // △
#define PAD_SQUARE      PAD_Y           // □
#define PAD_START       PAD_STA         // START
#define PAD_SELECT      PAD_SEL         // SELECT
#define PAD_L3          PAD_AL          // L3
#define PAD_R3          PAD_AR          // R3

/*---------------------------------------------------------------------------*/

//
//
//
//
//

#endif // {{{ END OF FILE }}}
