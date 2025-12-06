#include <stddef.h>     // for NULL
#include <eekernel.h>
#include <libgraph.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>

#include "sd_ee.h"
#include "jsifman.h"

#define CACHED(p)       ((void *)((unsigned int)(p) & 0x1fffffff))
#define UNCACHED(p)     ((void *)((unsigned int)(p) | 0x20000000))

/*---------------------------------------------------------------------------*/

//static u_char status[64] __attribute__((aligned(64)));
static u_char status[16] __attribute__((aligned(64)));

int sd_init(void)
{
    sif_init();
    sif_init_rv_man();

    while (!(sceSifGetSreg(8) & 0x2));
    sceGsSyncV(0);

    *(void **)status = status;
    sif_send_packet_and_data(1, 0, &status, 16, NULL, NULL, 0);
    return 1;
}

void sd_set_cli(int sound_code)
{
    ((u_int *)UNCACHED(status))[1] |= 0x80000000;
    sif_send_packet_and_data(1, sound_code, NULL, 0, NULL, NULL, 0);

    if ((sound_code & 0xFF000000) == 0xFE000000) {
        do {
            sceGsSyncV(0);
        } while (!(((u_int *)UNCACHED(status))[1] & 0x7));
    }

    if ((sound_code & 0xFF000000) == 0xF1000000) {
        ((u_int *)UNCACHED(status))[1] |= 0x1000;
    }

    if ((sound_code & 0xFF000000) == 0xF3000000) {
        ((u_int *)UNCACHED(status))[1] |= 0x2000;
    }
}

void *sd_status(void)
{
    return UNCACHED(status);
}
