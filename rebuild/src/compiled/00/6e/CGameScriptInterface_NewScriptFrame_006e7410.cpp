struct Guard {
    void* vt;    // +0x0
    char pad[1];
    char flag;   // +0x5
};
struct GuardVT {
    void (__fastcall *fn0)(void*);
    void (__fastcall *fn4)(void*);
    void (__fastcall *fn8)(void*);
};

struct Inner {
    char pad0[0x18];
    int f18;     // +0x18
    int f1c;     // +0x1c
};
struct Chain {
    char pad0[0x2c];
    Inner* p2c;  // +0x2c
};
struct Mid2 {
    char pad0[0x8];
    Chain* p8;   // +0x8
};
struct Big {
    char pad0[0x88];
    Mid2* p88;   // +0x88
};

extern Guard* g_guard;   // ds:0x13d2838
extern Big* g_big;       // ds:0x13b89fc

extern int __cdecl SomeCall(void);

void CGameScriptInterface_NewScriptFrame()
{
    Guard* g = g_guard;
    if (g->flag) return;
    ((GuardVT*)g->vt)->fn8(g);

    g = g_guard;
    if (g->flag) return;

    Inner* esi = g_big->p88->p8->p2c;
    int eax = SomeCall();
    esi->f18 = esi->f1c;
    esi->f1c = eax;
}