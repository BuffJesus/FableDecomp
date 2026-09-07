#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CResurrectionItemDef_Methods : CResurrectionItemDef {
    void Transfer(CPersistContext* ctx);
};
void CResurrectionItemDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }