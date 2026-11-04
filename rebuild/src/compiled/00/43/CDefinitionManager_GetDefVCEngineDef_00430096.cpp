#pragma optimize("s", on)
#include "fable_definition_manager.h"
#include "fable_engine_def.h"

// Look the def up by class name and class-local index (indices start at 1);
// on success share it with the caller's pointer.
template <class T>
bool CDefinitionManager::GetDef(const CCharString& name, long index, CDefPointer<const T>* output) const
{
    if (index > 0)
    {
        CDefPointer<const CDefClassBase> pdef = GetPDefFromClassIndex(name, index);
        if (pdef.Get())
        {
            *output = pdef;
            return true;
        }
    }
    return false;
}

template bool CDefinitionManager::GetDef<CEngineDef>(const CCharString& name, long index,
                                                      CDefPointer<const CEngineDef>* output) const;