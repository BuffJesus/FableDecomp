#include <cstdio>
typedef unsigned char u8;
typedef unsigned int u32;

struct TCEntry {
    int   key;
    void* iface;
};

struct HeroTarget {
    char  pad00[0x28];
    u32   field28;
    char  pad2C[0x18];
    char  mapStore[4];
    TCEntry* mapEnd;
    char  pad4C[0x45];
    u8    flags91;
};

struct GSISelf {
    char  pad00[0x14];
    void* field14;
};

struct TCMap {
    TCEntry* LowerBound(int* key);
};

// ---- test stubs ----
static HeroTarget* g_target = 0;
static TCEntry*    g_lb_result = 0;
static bool        g_getname_ret = false;
static void*       g_getname_arg = 0;

extern "C" void* __fastcall GSI_Helper_A(void* p) { return p; }
extern "C" HeroTarget* __fastcall GSI_Helper_B(void* p) { (void)p; return g_target; }
TCEntry* TCMap::LowerBound(int* key) { (void)key; return g_lb_result; }
extern "C" bool __fastcall TCShop_GetName(void* self) {
    g_getname_arg = self; return g_getname_ret;
}

bool __fastcall CGameScriptInterface_IsHeroNaked(const GSISelf* self)
{
    HeroTarget* target = GSI_Helper_B(GSI_Helper_A(self->field14));

    if (target != 0 &&
        (target->flags91 & 0x1) == 0 &&
        (target->field28 & 0x40000000) != 0)
    {
        char* base = (char*)target + 0x44;
        int key = 0x5E;
        TCEntry* e = ((TCMap*)base)->LowerBound(&key);
        TCEntry* end = *(TCEntry**)(base + 4);
        if (e == end || e->key > 0x5E)
            e = end;
        return TCShop_GetName(e->iface);
    }
    return false;
}

int main()
{
    GSISelf self;
    self.field14 = (void*)0x1234;

    // Case 1: null target -> false
    g_target = 0;
    if (CGameScriptInterface_IsHeroNaked(&self)) { std::printf("FAIL null\n"); return 1; }

    // Case 2: flags91 bit set -> false
    static HeroTarget t;
    for (u32 i = 0; i < sizeof(t); ++i) ((char*)&t)[i] = 0;
    t.field28 = 0x40000000;
    t.flags91 = 0x1;
    g_target = &t;
    if (CGameScriptInterface_IsHeroNaked(&self)) { std::printf("FAIL flag91\n"); return 1; }

    // Case 3: field28 bit clear -> false
    t.flags91 = 0;
    t.field28 = 0;
    if (CGameScriptInterface_IsHeroNaked(&self)) { std::printf("FAIL field28\n"); return 1; }

    // Case 4: valid, lower_bound found entry with key==0x5E -> use it, GetName true
    t.field28 = 0x40000000;
    static TCEntry found; found.key = 0x5E; found.iface = (void*)0xABCD;
    static TCEntry endEntry; endEntry.key = 0; endEntry.iface = (void*)0xEEEE;
    t.mapEnd = &endEntry;
    g_lb_result = &found;
    g_getname_ret = true;
    if (!CGameScriptInterface_IsHeroNaked(&self)) { std::printf("FAIL found\n"); return 1; }
    if (g_getname_arg != (void*)0xABCD) { std::printf("FAIL arg\n"); return 1; }

    // Case 5: lower_bound key > 0x5E -> falls back to end
    found.key = 0x5F;
    g_getname_ret = false;
    g_getname_arg = 0;
    if (CGameScriptInterface_IsHeroNaked(&self)) { std::printf("FAIL fallback\n"); return 1; }
    if (g_getname_arg != (void*)0xEEEE) { std::printf("FAIL endarg\n"); return 1; }

    // Case 6: lower_bound == end -> use end
    g_lb_result = &endEntry;
    found.key = 0x5E;
    if (CGameScriptInterface_IsHeroNaked(&self)) { std::printf("FAIL endeq\n"); return 1; }

    std::printf("CGameScriptInterface_0089b330_TEST PASS\n");
    return 0;
}