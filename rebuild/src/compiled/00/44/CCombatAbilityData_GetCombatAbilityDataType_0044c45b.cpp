// CCombatAbilityData::GetCombatAbilityDataType @ 0x0044c45b
// retail: xor eax,eax; ret  -> returns enum value 0 (this-call, no frame)

#include "engine/CCombatAbilityData.h"  // retyped onto the PDB layout; byte parity re-verified
enum ECombatAbilityDataType
{
    ECADT_Base = 0
};


// this-call member modeled as __fastcall free fn (self in ecx).
ECombatAbilityDataType __fastcall CCombatAbilityData_GetCombatAbilityDataType(CCombatAbilityData* self)
{
    return ECADT_Base;
}