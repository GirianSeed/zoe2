#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>

typedef struct {
    unsigned int paksize;   /* */
    unsigned int elfsize;   /* */
    unsigned int namelen;   /* strlen(name)+1 */
    unsigned int pad0;      /* padding */
} ipkModuleTag;

#define XORKEY_1STVAL   0x7e8a6b4c
#define XORKEY_ADDVAL   0xa84e6b2e

static u_long xor_work = XORKEY_1STVAL;

static void decode( u_long *dst, void *src, size_t size )
{
    int i;
    sceSifReceiveData rd;

    sceSifGetOtherData( &rd, src, dst, size, 0 );

    for (i = size; i > 0; dst++) {
        *dst ^= xor_work;
        i -= sizeof(u_long);
        xor_work += XORKEY_ADDVAL;
    }
}

static int iopLoadInit( void *src )
{
    ipkModuleTag modtag;    /* stack -0x78 */
    ModuleStatus modstat;   /* stack -0x68 */
    int oldstat;            /* stack -0x08 */
    int res;                /* stack -0x04 */

    int t;
    int modid;              /* $s0 */
    void *buffer;           /* $s1 */
    const char *filename;   /* $s2 */
    int args;               /* $s4 */
    const char *argp;       /* $s5 */

    while (1) {
        xor_work = XORKEY_1STVAL;
        decode( (u_long *)&modtag, src, sizeof(ipkModuleTag) );
        src += sizeof(ipkModuleTag);

        if (modtag.paksize == 0) {
            break;
        }
        CpuSuspendIntr( &oldstat );
        buffer = AllocSysMemory( SMEM_High, modtag.paksize, 0 );
        CpuResumeIntr( oldstat );

        if (buffer == NULL) {
            return -1;
        }
        decode( buffer, src, modtag.paksize );
        src += modtag.paksize;

        filename = (char *)(buffer + modtag.elfsize);
        argp = (char *)(filename + strlen( filename ) + 1);
        t = (modtag.namelen - 1);
        args = (t - strlen( filename ));

        modid = LoadModuleBuffer( buffer );
        FreeSysMemory( buffer );

        if (modid < 0) {
            continue;
        }
        ReferModuleStatus( modid, &modstat );
        res = StartModule( modid, filename, args, argp, &res );
    }
    return 0;
}

int start( int argc, char *argv[] )
{
    long addr = strtol( argv[1], NULL, 16 );

    sceSifInitRpc( 0 );

    if (iopLoadInit( (void *)addr ) < 0) {
        return KE_ERROR;
    } else {
        return NO_RESIDENT_END;
    }
}
