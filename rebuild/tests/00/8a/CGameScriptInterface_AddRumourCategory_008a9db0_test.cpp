#include <cstdio>

struct CCharString {
    unsigned int value;
    CCharString() : value(0) {}
    CCharString(const CCharString& other) : value(other.value) {}
};

static int g_findCalled = 0;
static unsigned int g_lastVal = 0;
void __stdcall CGameScriptInterface_FindOrCreateRumourCategory(CCharString category)
{
    g_findCalled++;
    g_lastVal = category.value;
}

struct CGameScriptInterface {
    void* vt;
    void AddRumourCategory(const CCharString& category) const;
};

void CGameScriptInterface::AddRumourCategory(const CCharString& category) const
{
    CGameScriptInterface_FindOrCreateRumourCategory(category);
}

int main(void)
{
    CGameScriptInterface gsi;
    gsi.vt = 0;
    CCharString cs;
    cs.value = 0xABCD1234u;
    gsi.AddRumourCategory(cs);
    if (g_findCalled == 1 && g_lastVal == 0xABCD1234u) {
        std::printf("CGameScriptInterface_008a9db0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL findCalled=%d val=%08x\n", g_findCalled, g_lastVal);
    return 1;
}