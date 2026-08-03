#include <cstdio>

struct Str {
    char* p;
    Str() { p = 0; }
    Str(const Str& o) { p = o.p; }
    ~Str() {}
};

static int g_called = 0;
static Str g_village;
static Str g_text;

void __cdecl AddGossipVillageEx(Str village, Str text)
{
    g_called++;
    g_village = village;
    g_text = text;
}

void __fastcall CGameScriptInterface_AddGossipVillage(void* self, const Str& gossipVillage, const Str& gossipText)
{
    Str textCopy(gossipText);
    Str villageCopy(gossipVillage);
    AddGossipVillageEx(villageCopy, textCopy);
}

int main() {
    Str village; village.p = (char*)0xAAAA;
    Str text; text.p = (char*)0xBBBB;

    CGameScriptInterface_AddGossipVillage((void*)0x1000, village, text);

    if (g_called == 1 && g_village.p == (char*)0xAAAA && g_text.p == (char*)0xBBBB) {
        std::printf("CGameScriptInterface_008A9E30_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d\n", g_called);
    return 1;
}