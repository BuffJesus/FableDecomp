#include "engine/CBuyHouseDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CBuyHouseDef_Methods : CBuyHouseDef {
    void Transfer(CPersistContext* ctx);
};
void CBuyHouseDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x25); }