#include <cstdio>

struct MidObj {
    char pad0[0xc];
    char flagC;
    unsigned char flagD;
    char pad_e[0xbf];
    char flagCD;
};

static MidObj g_mid;
MidObj* GetMidObj(void) { return &g_mid; }

static int g_calls;
static unsigned char g_lastB;
void __fastcall PlayMovie(void* target, int id,
    int a3, int a4, int a5, int a6, int a7, unsigned char b,
    float f, int a10, int a11, int a12)
{
    (void)target;(void)a3;(void)a4;(void)a5;(void)a6;(void)a7;(void)a10;(void)a11;(void)a12;
    g_calls++;
    g_lastB = b;
    if (id != 0x1b) { std::printf("FAIL id\n"); }
    if (f != -1.0f) { std::printf("FAIL f\n"); }
}

void __fastcall CGameScriptInterface_PlayAVIMovie(void* self, void* edx_, void* target)
{
    (void)self; (void)edx_;
    MidObj* m = GetMidObj();
    PlayMovie(target, 0x1b, 0, 0, 0, 0, 0,
              m->flagC ? (unsigned char)m->flagCD : m->flagD, -1.0f, 0, 0, 0);
}

int main() {
    g_mid.flagC = 0; g_mid.flagD = 0x42; g_mid.flagCD = 0x77;
    CGameScriptInterface_PlayAVIMovie((void*)0x1000, 0, (void*)0x2000);
    if (g_lastB != 0x42) { std::printf("FAIL path0\n"); return 1; }
    g_mid.flagC = 1;
    CGameScriptInterface_PlayAVIMovie((void*)0x1000, 0, (void*)0x2000);
    if (g_lastB != 0x77) { std::printf("FAIL path1\n"); return 1; }
    if (g_calls != 2) { std::printf("FAIL calls\n"); return 1; }
    std::printf("CGameScriptInterface_0088f890_TEST PASS\n");
    return 0;
}