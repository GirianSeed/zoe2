#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <eekernel.h>
#include <sifrpc.h>
#include <libdev.h>
#include <libdma.h>
#include <libgraph.h>

// from module/mts/mts.c
extern "C" void MTS_BootThread( char *name, void (*func)(), int pri,
                void *stack_top, int stack_size, void *arg );

/*---------------------------------------------------------------------------*/
// local to main/main.cc

extern int boot_th_id; //= 0;
extern int main_argc; //= 0;
extern char **main_argv; //= NULL;

#define STACK_SIZE (16 * 1024)

extern u_long128 main_stack[ STACK_SIZE / sizeof(u_long128) ];
extern void Main();

/*---------------------------------------------------------------------------*/

int main( int argc, char *argv[] )
{
    main_argc = argc;
    main_argv = argv;
    boot_th_id = GetThreadId();

    MTS_BootThread( "ZOE2MAIN", Main, 18, main_stack, STACK_SIZE, NULL );
    for (;;) {
        printf( "ExitMainLoop\n" );
        SleepThread();
    }
}
