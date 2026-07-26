#include <cstdio>

struct CCarrySlotDef { char pad[4]; };

static int g_called = 0;
static CCarrySlotDef* g_self = 0;
static void* g_src = 0;

void __fastcall CCarrySlotDef_CopyImpl(CCarrySlotDef* self, void* src)
{
    g_called = 1;
    g_self = self;
    g_src = src;
}

void __fastcall CCarrySlotDef_Copy(CCarrySlotDef* self, void* src)
{
    CCarrySlotDef_CopyImpl(self, src);
}

int main()
{
    CCarrySlotDef d;
    int srcval = 42;
    CCarrySlotDef_Copy(&d, &srcval);
    if (g_called == 1 && g_self == &d && g_src == &srcval) {
        std::printf("CCarrySlotDef_00453810_TEST PASS\n");
        return 0;
    }
    std::printf("CCarrySlotDef_00453810_TEST FAIL\n");
    return 1;
}