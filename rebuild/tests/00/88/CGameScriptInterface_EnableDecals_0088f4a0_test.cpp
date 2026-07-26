#include <cstdio>

struct GObj { char pad[0x1010]; unsigned char b0:1; unsigned char b1:1; };
static GObj g_obj;
GObj* GetDecalObj(void) { return &g_obj; }

struct CGameScriptInterface_EnableDecals_cls {
    void CGameScriptInterface_EnableDecals(char enable);
};

void CGameScriptInterface_EnableDecals_cls::CGameScriptInterface_EnableDecals(char enable)
{
    GObj* o = GetDecalObj();
    o->b1 = enable;
}

int main()
{
    CGameScriptInterface_EnableDecals_cls obj;
    unsigned char* raw = (unsigned char*)&g_obj + 0x1010;
    *raw = 0x00;
    obj.CGameScriptInterface_EnableDecals(1);
    if ((*raw & 0x2) != 0x2) { std::printf("FAIL set %02x\n", *raw); return 1; }
    *raw = 0xFD;
    obj.CGameScriptInterface_EnableDecals(0);
    if (*raw != 0xFD) { std::printf("FAIL clear-preserve %02x\n", *raw); return 1; }
    *raw = 0xFD;
    obj.CGameScriptInterface_EnableDecals(1);
    if (*raw != 0xFF) { std::printf("FAIL set-preserve %02x\n", *raw); return 1; }
    std::printf("CGameScriptInterface_0088f4a0_TEST PASS\n");
    return 0;
}