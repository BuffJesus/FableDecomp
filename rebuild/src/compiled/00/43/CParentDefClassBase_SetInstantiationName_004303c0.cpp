// CParentDefClassBase::SetInstantiationName(CDefString) @ 0x004303C0 (FableWin
// ?SetInstantiationName@CParentDefClassBase@@UAEXVCDefString@@@Z; vtable slot
// 11; called by CDefinitionManager::InstantiateDef 0x009ADC90).
#include "fable_def_class_base.h"

// Name the def.
void CParentDefClassBase::SetInstantiationName(CDefString name)
{
    InstantiationName = name;
}