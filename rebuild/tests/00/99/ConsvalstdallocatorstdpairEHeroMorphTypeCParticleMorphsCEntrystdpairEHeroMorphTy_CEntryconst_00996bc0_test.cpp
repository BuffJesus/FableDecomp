#include <stdio.h>

enum EHeroMorphType { HMT_NONE };

struct CEntry {
    int a;
    int b;
    int c;
};

struct Pair {
    EHeroMorphType first;
    CEntry second;
};

struct InnerAlloc {
    void Cons_val(Pair* p, const Pair* v);
};

struct OuterAlloc {
    int pad0;
    int pad4;
    InnerAlloc inner;   // at +8

    void Cons_val(Pair* p, const Pair* v);
};

// --- captured effect for the standalone test ---
static InnerAlloc* g_seen_self = 0;
static Pair*       g_seen_p    = 0;
static const Pair* g_seen_v    = 0;

void InnerAlloc::Cons_val(Pair* p, const Pair* v)
{
    g_seen_self = this;
    g_seen_p = p;
    g_seen_v = v;
    // copy-construct semantics stand-in
    p->first = v->first;
    p->second = v->second;
}

void OuterAlloc::Cons_val(Pair* p, const Pair* v)
{
    inner.Cons_val(p, v);
}

int main()
{
    OuterAlloc outer;
    outer.pad0 = 0x11111111;
    outer.pad4 = 0x22222222;

    Pair dst;
    Pair src;
    src.first = HMT_NONE;
    src.second.a = 7;
    src.second.b = 8;
    src.second.c = 9;

    dst.second.a = 0;

    outer.Cons_val(&dst, &src);

    bool ok = true;
    // forwarder must have shifted `this` to inner subobject (outer + 8 bytes)
    if (g_seen_self != &outer.inner) ok = false;
    if ((char*)g_seen_self != (char*)&outer + 8) ok = false;
    if (g_seen_p != &dst) ok = false;
    if (g_seen_v != &src) ok = false;
    if (dst.second.a != 7 || dst.second.b != 8 || dst.second.c != 9) ok = false;

    if (ok) printf("CONS_VAL_FWD_OK\n");
    else    printf("CONS_VAL_FWD_FAIL self=%p p=%p v=%p\n",
                   (void*)g_seen_self, (void*)g_seen_p, (void*)g_seen_v);
    return ok ? 0 : 1;
}