#include "rebuild_abi.h"
#include <cstdio>

struct Obj_00428e8b {
    void* field0;
};

Obj_00428e8b* __fastcall set_field0_00428e8b(Obj_00428e8b* thisptr, int edx_dummy, void* value);

Obj_00428e8b* __fastcall set_field0_00428e8b(Obj_00428e8b* thisptr, int /*edx_dummy*/, void* value)
{
    thisptr->field0 = value;
    return thisptr;
}

int main()
{
    Obj_00428e8b o;
    o.field0 = (void*)0;
    int marker = 0x1234;
    Obj_00428e8b* ret = set_field0_00428e8b(&o, 0, &marker);
    if (ret == &o && o.field0 == (void*)&marker) {
        printf("OK_428e8b\n");
    } else {
        printf("FAIL_428e8b\n");
    }
    return 0;
}