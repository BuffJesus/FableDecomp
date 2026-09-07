#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CCameraCollisionDef_Methods : CCameraCollisionDef {
    void Transfer(CPersistContext* ctx);
};
void CCameraCollisionDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }