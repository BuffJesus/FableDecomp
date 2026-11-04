// CFrontEndDef::Alloc() @ 0x00433080 (FableWin ?Alloc@CFrontEndDef@@SIPAVCDefClassBase@@XZ;
// the FRONT_END class allocator CNewFrontendGameComponent::CompileDefsPlatformSpecific
// 0x0042F5A9 registers). The inline ctors run CResource's out-of-line one
// (0x009FBEC0), then the def-class, parent-def and front-end members.
#include "fable_frontend_def.h"

FABLE_STATIC_ASSERT(sizeof(CFrontEndDef) == 0x54);

// A new, unnamed front-end def (null if the allocation fails).
CDefClassBase* FABLE_FASTCALL CFrontEndDef::Alloc()
{
    return new CFrontEndDef;
}