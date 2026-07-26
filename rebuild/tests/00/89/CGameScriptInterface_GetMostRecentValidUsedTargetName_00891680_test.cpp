#include <cstdio>

struct CCharString {
    void* p;
    CCharString() { p = 0; }
    CCharString(const CCharString& o) { p = o.p; }
};

struct CGlobal { char pad[0xb0]; CCharString name; };
static CGlobal g_storage;
CGlobal* g_obj = &g_storage;

struct CGameScriptInterface {
    char pad[0x1010];
    CCharString GetMostRecentValidUsedTargetName();
};

CCharString CGameScriptInterface::GetMostRecentValidUsedTargetName()
{
    return g_obj->name;
}

int main() {
    g_storage.name.p = (void*)0xDEADBEEF;
    CGameScriptInterface gsi;
    CCharString r = gsi.GetMostRecentValidUsedTargetName();
    if (r.p == (void*)0xDEADBEEF) {
        std::printf("CGameScriptInterface_00891680_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %p\n", r.p);
    return 1;
}