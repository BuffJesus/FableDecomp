#include "rebuild_abi.h"

struct Obj_00428e8b {
    void* field0;
};

Obj_00428e8b* __fastcall set_field0_00428e8b(Obj_00428e8b* thisptr, int /*edx_dummy*/, void* value)
{
    thisptr->field0 = value;
    return thisptr;
}