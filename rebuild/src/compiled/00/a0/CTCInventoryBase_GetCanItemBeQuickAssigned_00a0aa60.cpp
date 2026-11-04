#include "fable_render_state_manager.h"

// Retail 0x00A0AA60 reads CRenderManagerCore::Rendering (+0x3A54). The
// propagated manifest label (NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned)
// cannot own this body: CTCInventoryBase is 0x148 bytes with that flag at +0x3C.
bool CRenderManagerCore::IsRendering()
{
    return Rendering;
}
