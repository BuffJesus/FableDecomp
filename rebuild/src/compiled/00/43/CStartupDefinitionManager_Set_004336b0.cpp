// CStartupDefinitionManager::Set, retail 0x004336b0 (FableWin ?Set@CStartupDefinitionManager@@SIXPAV1@@Z): hand the singleton pointer
// to CStartupDefinitionManager::PClass (CCountedPointer::Reset takes ownership).
#include "fable_definition_manager.h"

// Retail 0x013B8760 (the startup manager's unit owns the singleton; the Play
// seam needs it defined, since an unresolved static aliases shared zero storage).
CCountedPointer<CStartupDefinitionManager> CStartupDefinitionManager::PClass;

void CStartupDefinitionManager::Set(CStartupDefinitionManager* manager)
{
    PClass.Reset(manager);
}