
#include <cstdio>

struct Str {
    char* p;
    Str() { p = 0; }
    Str(const Str& o) { p = o.p; }
    ~Str() {}
};

static int g_called = 0;
static Str* g_arg = 0;
struct GObj {
    void RemoveGV(Str* s) { g_called++; g_arg = s; }
};

// Placeholder object so the constant-address call is valid at runtime.
static GObj real_obj;

void __fastcall CGameScriptInterface_RemoveGossipVillages(void* self, Str param)
{
    Str tmp(param);
    real_obj.RemoveGV(&tmp);   // behavior mirror (constant-addr call in src version)
}

int main() {
    Str s; s.p = (char*)0xDEAD;
    CGameScriptInterface_RemoveGossipVillages((void*)0x1000, s);
    if (g_called == 1 && g_arg != 0) {
        std::printf("CGameScriptInterface_008aa2b0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d\n", g_called);
    return 1;
}