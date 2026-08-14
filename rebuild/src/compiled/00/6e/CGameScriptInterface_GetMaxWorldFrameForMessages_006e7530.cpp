// Byte-exact reconstruction of CGameScriptInterface::GetMaxWorldFrameForMessages @ 0x006e7530
// Reads a global singleton, walks a pointer chain, returns a long.

struct D { long dummy[7]; long frame; };      // +0x1c
struct C { char pad[0x2c]; D* pD; };           // +0x2c
struct B { char pad0[8]; C* pC; };             // +0x08
struct A { char pad0[0x88]; B* pB; };          // +0x88

struct CGameScriptInterface;

A* g_singleton = 0;   // at 0x13b89fc

long __fastcall GetMaxWorldFrameForMessages(CGameScriptInterface* self)
{
    (void)self;
    B* b = g_singleton->pB;         // [0x13b89fc] -> [+0x88]
    if (b) {
        C* c = b->pC;               // [+8]
        if (c) {
            D* d = c->pD;           // [+0x2c]
            return d->frame;        // [+0x1c]
        }
    }
    return 0;
}