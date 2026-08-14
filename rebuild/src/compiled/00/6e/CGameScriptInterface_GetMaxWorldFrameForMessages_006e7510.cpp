// Byte-exact reconstruction of CGameScriptInterface::GetMaxWorldFrameForMessages @ 0x006e7510
// this-call member; body ignores this and reads a global chain.

struct L3 { char pad18[0x18]; long value; };          // +0x18
struct L2 { char pad2c[0x2c]; L3* p2c; };              // +0x2c
struct L1 { char pad08[0x08]; L2* p08; };              // +0x08
struct Root { char pad88[0x88]; L1* p88; };            // +0x88

struct CGameScriptInterface;

extern Root* g_root;   // global at 0x13b89fc

long __fastcall GetMaxWorldFrameForMessages(CGameScriptInterface* self)
{
    L1* a = g_root->p88;
    if (a) {
        L2* b = a->p08;
        if (b) {
            return b->p2c->value;
        }
    }
    return 0;
}