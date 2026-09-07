#include "engine/CAICreatureWillPowerIndicatorDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CAICreatureWillPowerIndicatorDef_Methods : CAICreatureWillPowerIndicatorDef {
    void Transfer(CPersistContext* ctx);
};
void CAICreatureWillPowerIndicatorDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }