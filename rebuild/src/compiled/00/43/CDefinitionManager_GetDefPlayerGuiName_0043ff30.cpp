#include "fable_definition_manager.h"
class CPlayerGuiDef;
template<class T> bool CDefinitionManager::GetDef(const CCharString& name,CDefPointer<const T>* output) const
{
        CDefPointer<const CDefClassBase> def=GetPDefFromInstantiationName(name);
        if(def.Get())
        {
            *output=def;
            return true;
        }
    return false;
}
template bool CDefinitionManager::GetDef<CPlayerGuiDef>(const CCharString&,CDefPointer<const CPlayerGuiDef>*) const;
