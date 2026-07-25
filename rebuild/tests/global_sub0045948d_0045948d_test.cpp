#include "rebuild_abi.h"
#include <stdio.h>

struct Obj_0045948d;

extern "C" void* __cdecl op_new_0045948d(unsigned int size);
extern void __fastcall ctor_0045948d(Obj_0045948d* thisptr);
extern const void* vtbl_0045948d;

struct Obj_0045948d
{
    const void* vptr;
};

Obj_0045948d* __fastcall sub_0045948d();

// ---- mocks for relocation-masked callees/globals ----
static char g_storage[0x74];
static int g_ctor_called = 0;
static unsigned int g_new_size = 0;
const void* vtbl_0045948d = 0;

extern "C" void* __cdecl op_new_0045948d(unsigned int size)
{
    g_new_size = size;
    return (void*)g_storage;
}

void __fastcall ctor_0045948d(Obj_0045948d* thisptr)
{
    (void)thisptr;
    g_ctor_called = 1;
}

int main()
{
    Obj_0045948d* r = sub_0045948d();
    if (r == (Obj_0045948d*)g_storage &&
        g_new_size == 0x74 &&
        g_ctor_called == 1 &&
        r->vptr == &vtbl_0045948d)
    {
        printf("PASS_0045948d_OK\n");
    }
    else
    {
        printf("FAIL_0045948d\n");
    }
    return 0;
}