#include "engine/CGoldDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CGoldDef_Methods : CGoldDef {
    void Transfer(CPersistContext* ctx);
};
void CGoldDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }