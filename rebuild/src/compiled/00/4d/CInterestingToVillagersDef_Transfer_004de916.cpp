#include "engine/CInterestingToVillagersDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CInterestingToVillagersDef_Methods : CInterestingToVillagersDef {
    void Transfer(CPersistContext* ctx);
};
void CInterestingToVillagersDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }