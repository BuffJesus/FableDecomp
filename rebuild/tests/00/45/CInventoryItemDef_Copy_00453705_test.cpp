#include <cstdio>
struct CInventoryItemDef { int v; };
static int g_called = 0;
static CInventoryItemDef* g_self = 0;
static void* g_src = 0;
void __fastcall CInventoryItemDef_Copy_impl(CInventoryItemDef* self, void* src) {
    g_called = 1; g_self = self; g_src = src;
}
void __fastcall CInventoryItemDef_Copy(CInventoryItemDef* self, void* src) {
    CInventoryItemDef_Copy_impl(self, src);
}
int main() {
    CInventoryItemDef obj; obj.v = 7;
    int dummy = 42;
    CInventoryItemDef_Copy(&obj, &dummy);
    if (g_called == 1 && g_self == &obj && g_src == &dummy) {
        std::printf("CInventoryItemDef_00453705_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}