#include <cstdio>

// Independent behaviour test. Provide local defs of the extern helpers so it
// links standalone, and observe the destroy order + tail-call.

struct CCharString {
    char* m_ptr;
    int   m_len;
    void Destroy();
};

struct Pair;
void __fastcall PairDestroyFirst(Pair* self);

struct Pair {
    char        pad0[0x14];
    CCharString s_a;   // +0x14
    CCharString s_b;   // +0x1c
};

void __fastcall Dest_val(Pair* self)
{
    self->s_b.Destroy();
    self->s_a.Destroy();
    PairDestroyFirst(self);
}

// ---- instrumentation ----
static int g_seq = 0;
static int g_a_at = 0, g_b_at = 0, g_first_at = 0;
static Pair* g_obj = 0;

void CCharString::Destroy()
{
    // which member? compare address against the live object
    if (this == &g_obj->s_a) g_a_at = ++g_seq;
    else if (this == &g_obj->s_b) g_b_at = ++g_seq;
}

void __fastcall PairDestroyFirst(Pair* self)
{
    g_first_at = ++g_seq;
}

int main()
{
    Pair p;
    p.s_a.m_ptr = 0; p.s_a.m_len = 0;
    p.s_b.m_ptr = 0; p.s_b.m_len = 0;
    g_obj = &p;

    Dest_val(&p);

    // Retail order: +0x1c (s_b) first, then +0x14 (s_a), then first-destroy.
    bool ok = (g_b_at == 1) && (g_a_at == 2) && (g_first_at == 3);

    // sanity: offsets match the retail disasm
    unsigned base = (unsigned)(void*)&p;
    ok = ok && ((unsigned)(void*)&p.s_a - base == 0x14);
    ok = ok && ((unsigned)(void*)&p.s_b - base == 0x1c);

    if (ok) printf("DESTVAL_ORDER_OK\n");
    else    printf("FAIL b=%d a=%d f=%d\n", g_b_at, g_a_at, g_first_at);
    return ok ? 0 : 1;
}