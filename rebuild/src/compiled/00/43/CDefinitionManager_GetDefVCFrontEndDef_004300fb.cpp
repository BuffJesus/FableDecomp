// CDefinitionManager::GetDef<CFrontEndDef>(const CCharString&, long,
// CDefPointer<const CFrontEndDef>*) const @ 0x004300FB (FableWin
// ??$GetDef@VCFrontEndDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCFrontEndDef@@@@@Z;
// the linker folds every GetDef<T> with this body onto it).
// CNewFrontendGameComponent::InitialiseDefs 0x0042F722 calls it; retail's unit
// is size-optimised (the ebp frame, `and [esi],0`).
#pragma optimize("s", on)
#include "fable_definition_manager.h"
#include "fable_frontend_def.h"

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

template bool CDefinitionManager::GetDef<CFrontEndDef>(const CCharString& name, long index,
                                                      CDefPointer<const CFrontEndDef>* output) const;