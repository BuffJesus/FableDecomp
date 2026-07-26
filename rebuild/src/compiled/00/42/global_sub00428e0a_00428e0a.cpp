#include "rebuild_abi.h"

// Retail 0x00428e0a: __fastcall accessor modeled as __fastcall(this, edx, value).
//   mov eax, ecx          ; eax = this
//   mov ecx, [esp+4]      ; value (first stack arg)
//   mov [eax], ecx        ; this->field0 = value
//   ret 4                 ; returns this, pops the stack arg
struct Obj_00428e0a {
    void* field0;
};

Obj_00428e0a* __fastcall sub_00428e0a(Obj_00428e0a* thisptr, int edx, void* value)
{
    thisptr->field0 = value;
    return thisptr;
}