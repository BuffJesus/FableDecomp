// CDefinitionManager::ClearInstantiatedDefs() @ 0x009AD950 (FableWin
// ?ClearInstantiatedDefs@CDefinitionManager@@IAEXXZ): deletes every
// instantiated def's record (releasing its def) and empties each def class's
// list of global indices; the slots themselves stay, nulled.
#include "fable_definition_manager.h"

void CDefinitionManager::ClearInstantiatedDefs()
{
    for (CInstantiatedDefInfo** it = InstantiatedDefs.begin(); it != InstantiatedDefs.end(); ++it)
    {
        delete *it;
        *it = 0;
    }
    for (_STL::pair<unsigned long, CDefClassInfo>* entry = DefClassInfo.begin(); entry != DefClassInfo.end(); ++entry)
        entry->second.GlobalIndices.clear();
}
