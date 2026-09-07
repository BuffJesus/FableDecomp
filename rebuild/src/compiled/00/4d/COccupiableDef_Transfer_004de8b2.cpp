#include "engine/COccupiableDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct COccupiableDef_Methods : COccupiableDef {
    void Transfer(CPersistContext* ctx);
};
void COccupiableDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }