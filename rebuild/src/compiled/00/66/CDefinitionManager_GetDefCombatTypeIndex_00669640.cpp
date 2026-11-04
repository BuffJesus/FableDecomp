#include "fable_definition_manager.h"
#include "fable_creature_combat.h"
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* output) const
{
    if(index>0)
    {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if(def.Get())
        {
            *output=def;
            return true;
        }
    }
    return false;
}
template bool CDefinitionManager::GetDef<CCombatTypeDef>(long,CDefPointer<const CCombatTypeDef>*) const;
