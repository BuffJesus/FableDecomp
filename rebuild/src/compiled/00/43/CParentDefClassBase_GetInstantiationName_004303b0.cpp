// CParentDefClassBase::GetInstantiationName() const @ 0x004303B0 (FableWin
// ?GetInstantiationName@CParentDefClassBase@@UBE?AVCDefString@@XZ; vtable
// slot 10). Genuine C++ replacing the former machine-code bake.
#include "fable_def_class_base.h"

// The def's own name (a parent def is instantiated under one).
CDefString CParentDefClassBase::GetInstantiationName() const
{
    return InstantiationName;
}