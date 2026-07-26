#include "rebuild_abi.h"
#include <stdio.h>

extern void* const vtbl_0041cd01[];
void* const vtbl_0041cd01[1] = { 0 };

struct Obj_0041cd01 {
    void* vtbl;
};

Obj_0041cd01* __fastcall sub_0041cd01(Obj_0041cd01* thisptr);

Obj_0041cd01* __fastcall sub_0041cd01(Obj_0041cd01* thisptr)
{
    thisptr->vtbl = (void*)vtbl_0041cd01;
    return thisptr;
}

int main()
{
    Obj_0041cd01 o;
    o.vtbl = 0;
    Obj_0041cd01* r = sub_0041cd01(&o);
    if (r == &o && o.vtbl == (void*)vtbl_0041cd01)
        printf("PASS_0041cd01\n");
    else
        printf("FAIL_0041cd01\n");
    return 0;
}