#include <cstdio>

struct HelperResult {
    char pad[0x20];
    void* f20;
};

struct CThingCreatureBase;

extern HelperResult* __fastcall helper_686a80(CThingCreatureBase* self);

void* __fastcall GetCombatManager(CThingCreatureBase* self)
{
    return helper_686a80(self)->f20;
}

static HelperResult g_res;
static CThingCreatureBase* g_expected_self;

HelperResult* __fastcall helper_686a80(CThingCreatureBase* self)
{
    if (self != g_expected_self) { std::printf("BAD SELF\n"); }
    return &g_res;
}

int main()
{
    void* marker = (void*)0xDEADBEEF;
    g_res.f20 = marker;
    g_expected_self = (CThingCreatureBase*)0x12345678;
    void* r = GetCombatManager((CThingCreatureBase*)0x12345678);
    if (r == marker) {
        std::printf("006afd40_TEST PASS\n");
    } else {
        std::printf("FAIL got %p\n", r);
    }
    return 0;
}