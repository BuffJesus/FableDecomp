#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext { void Xfer(void* sub); };
struct CVillageMemberDef_Methods : CVillageMemberDef {
    void Transfer(CPersistContext* ctx);
};
void CVillageMemberDef_Methods::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x25); }