// Byte-exact reconstruction of LHHttp2::IsDataAvailable @ 0x0083ea50

#include "engine/LHHttp2.h"  // retyped onto the PDB layout; byte parity re-verified
typedef int LH_RETURN;

// callee @ 0x8f8210 : __fastcall method on the sub-object (this in ecx),
// takes one stack arg, returns char/bool in al. Modeled as __fastcall free fn.
struct LSocket {
    char query(int flag); // __fastcall -> this in ecx, flag on stack
};


// __fastcall modeled as __fastcall free fn (self in ecx)
LH_RETURN __fastcall IsDataAvailable(LHHttp2* self)
{
    LSocket* c = *(LSocket**)self; // mov ecx,[ecx]
    if (c != 0)
        return c->query(0) ? 0 : 3;
    return 2;
}