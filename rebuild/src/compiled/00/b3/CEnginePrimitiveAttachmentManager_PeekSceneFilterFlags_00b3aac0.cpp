#include "engine/CEnginePrimitiveAttachmentManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEnginePrimitiveAttachmentManager_Methods : CEnginePrimitiveAttachmentManager {
    unsigned long __fastcall PeekSceneFilterFlags();
};

unsigned long __fastcall CEnginePrimitiveAttachmentManager_Methods::PeekSceneFilterFlags()
{
    return 3;
}