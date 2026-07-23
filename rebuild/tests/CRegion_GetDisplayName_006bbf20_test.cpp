#include <cstdio>

static bool g_result = true;
static const void* g_lastSelf = 0;
static const char* g_lastArg = 0;

struct CCharString {
    char* data;
    bool Compare(const char* s) const;
};

static const char kEmptyDisplayNameTag[] = "TAG";

bool CCharString::Compare(const char* s) const {
    g_lastSelf = this;
    g_lastArg = s;
    return g_result;
}

struct CRegion {
    char pad[0x18];
    CCharString nameA;
    CCharString nameB;
};

CCharString* __fastcall CRegion_GetDisplayName(CRegion* self)
{
    CCharString* pB = &self->nameB;
    if (pB->Compare(kEmptyDisplayNameTag))
        return pB;
    return &self->nameA;
}

int main()
{
    CRegion r;
    g_result = true;
    CCharString* ret = CRegion_GetDisplayName(&r);
    if (ret != &r.nameB) { std::printf("FAIL: expected nameB\n"); return 1; }
    if (g_lastSelf != (const void*)&r.nameB) { std::printf("FAIL: wrong self\n"); return 1; }
    if (g_lastArg != kEmptyDisplayNameTag) { std::printf("FAIL: wrong arg\n"); return 1; }
    g_result = false;
    ret = CRegion_GetDisplayName(&r);
    if (ret != &r.nameA) { std::printf("FAIL: expected nameA\n"); return 1; }
    std::printf("CRegion_006bbf20_TEST PASS\n");
    return 0;
}