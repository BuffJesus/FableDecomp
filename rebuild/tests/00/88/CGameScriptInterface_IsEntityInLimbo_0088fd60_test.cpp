#include <cstdio>

struct Inner {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
};
struct CGameScriptInterface {
    Inner* (__fastcall **p0)(void*);
};

bool __stdcall CGameScriptInterface_IsEntityInLimbo(CGameScriptInterface* self)
{
    Inner* (__fastcall **vt)(void*) = self->p0;
    Inner* e = (*(Inner* (__fastcall **)(void*))((char*)vt + 0x2c))(self);
    if (e == 0)
        return false;
    unsigned char f = e->flags;
    if (!(f & 1))
        return (f & 0x20) ? true : false;
    return false;
}

static Inner g_inner;
static Inner* g_ret;

Inner* __fastcall vfn(void* /*ecx*/)
{
    return g_ret;
}

int main()
{
    void* vtbl[16];
    for (int i = 0; i < 16; ++i) vtbl[i] = (void*)&vfn;
    CGameScriptInterface self;
    self.p0 = (Inner* (__fastcall **)(void*))&vtbl[0];

    // case 1: null entity -> false
    g_ret = 0;
    if (CGameScriptInterface_IsEntityInLimbo(&self) != false) { std::printf("FAIL null\n"); return 1; }

    g_ret = &g_inner;

    // case 2: bit0 set -> false
    g_inner.flags = 0x01;
    if (CGameScriptInterface_IsEntityInLimbo(&self) != false) { std::printf("FAIL bit0\n"); return 1; }

    // case 3: bit5 set, bit0 clear -> true
    g_inner.flags = 0x20;
    if (CGameScriptInterface_IsEntityInLimbo(&self) != true) { std::printf("FAIL bit5true\n"); return 1; }

    // case 4: bit5 clear, bit0 clear -> false
    g_inner.flags = 0x00;
    if (CGameScriptInterface_IsEntityInLimbo(&self) != false) { std::printf("FAIL bit5false\n"); return 1; }

    // case 5: bit0 wins over bit5
    g_inner.flags = 0x21;
    if (CGameScriptInterface_IsEntityInLimbo(&self) != false) { std::printf("FAIL both\n"); return 1; }

    // case 6: high bits set but bit0/bit5 clear -> false
    g_inner.flags = 0xDA; // 11011010: bit0=0, bit5=0
    if (CGameScriptInterface_IsEntityInLimbo(&self) != false) { std::printf("FAIL high\n"); return 1; }

    std::printf("CGameScriptInterface_0088fd60_TEST PASS\n");
    return 0;
}