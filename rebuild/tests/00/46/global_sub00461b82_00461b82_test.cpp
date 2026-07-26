#include "rebuild_abi.h"
#include <stdio.h>

struct SomeType { int x; };

static unsigned int g_last_size = 0;
static SomeType g_obj;

extern "C" void* __cdecl engine_op_new_461b82(unsigned int size)
{
    g_last_size = size;
    return &g_obj;
}
extern SomeType* __fastcall SomeType_ctor_461b82(SomeType* thisptr)
{
    thisptr->x = 0x1234;
    return thisptr;
}

extern SomeType* __fastcall make_461b82(void);

int main(void)
{
    SomeType* r = make_461b82();
    if (r == &g_obj && g_last_size == 0x7c && r->x == 0x1234)
        printf("PASS_461b82_OK\n");
    else
        printf("FAIL_461b82\n");
    return 0;
}