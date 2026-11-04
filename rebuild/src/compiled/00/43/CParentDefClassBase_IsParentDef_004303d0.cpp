// CParentDefClassBase::IsParentDef() const @ 0x004303D0 (FableWin
// ?IsParentDef@CParentDefClassBase@@UBE_NXZ; vtable slot 16; asked by
// CDefinitionManager::InstantiateDef 0x009ADC90).
#include "fable_def_class_base.h"

// Parent defs are named on instantiation.
bool CParentDefClassBase::IsParentDef() const
{
    return true;
}