#include "rebuild_abi.h"
#include <stdio.h>

extern void* g_vtable_00404860;
void* g_vtable_00404860 = 0;

struct Obj_00404860 { void* vtbl; };

Obj_00404860* __fastcall sub_00404860(Obj_00404860* thisptr)
{
    *(void**)thisptr = &g_vtable_00404860;
    return thisptr;
}

int main()
{
    Obj_00404860 o;
    o.vtbl = 0;
    Obj_00404860* r = sub_00404860(&o);
    if (r == &o && o.vtbl == (void*)&g_vtable_00404860)
        printf("PASS_00404860_OK\n");
    else
        printf("FAIL_00404860\n");
    return 0;
}