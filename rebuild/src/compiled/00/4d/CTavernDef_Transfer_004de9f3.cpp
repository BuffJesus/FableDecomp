#include "engine/CTavernDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CTavernDef_Methods : CTavernDef {
    void Transfer(CPersistContext* ctx);
};
void CTavernDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }