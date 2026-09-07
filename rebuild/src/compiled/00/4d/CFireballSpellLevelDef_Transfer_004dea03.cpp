#include "engine/CFireballSpellLevelDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CFireballSpellLevelDef_Methods : CFireballSpellLevelDef {
    void Transfer(CPersistContext* ctx);
};
void CFireballSpellLevelDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }