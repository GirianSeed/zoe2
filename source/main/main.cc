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

// local to main/main.cc
extern int main_thid; //= 0;
extern int main_argc; //= 0;
extern char **main_argv; //= NULL;

extern char main_stack[0x4000];
extern void Main();

int main( int argc, char *argv[] )
{
    main_argc = argc;
    main_argv = argv;
    main_thid = GetThreadId();

    MTS_BootThread( "ZOE2MAIN", Main, 18, main_stack, 0x4000, NULL );
    while (1) {
        printf( "ExitMainLoop\n" );
        SleepThread();
    }
}
