#include "engine/CEnemyDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CEnemyDef_Methods : CEnemyDef {
    void Transfer(CPersistContext* ctx);
};
void CEnemyDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }