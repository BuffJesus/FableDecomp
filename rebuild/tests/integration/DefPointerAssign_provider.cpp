#include "fable_def_pointer.h"
class CCombatTypeDef;
typedef CDefPointer<const CCombatTypeDef> CombatDefPointer;
extern "C" CombatDefPointer& __fastcall AssignDefPointer(CombatDefPointer& destination, const CombatDefPointer& source)
{
    return destination = source;
}
