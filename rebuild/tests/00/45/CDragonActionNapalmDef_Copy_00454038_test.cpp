#include <cstdio>
struct CDragonActionNapalmDef { int v; };
static int g_called = 0;
static void* g_last_self = 0;
static void* g_last_src = 0;
void __fastcall base_copy(CDragonActionNapalmDef* self, void* src)
{
    g_called = 1;
    g_last_self = self;
    g_last_src = src;
}
void __fastcall CDragonActionNapalmDef_Copy(CDragonActionNapalmDef* self, void* src)
{
    base_copy(self, src);
}
int main()
{
    CDragonActionNapalmDef obj; obj.v = 7;
    int src = 42;
    CDragonActionNapalmDef_Copy(&obj, &src);
    if (g_called == 1 && g_last_self == (void*)&obj && g_last_src == (void*)&src) {
        std::printf("CDragonActionNapalmDef_00454038_TEST PASS\n");
        return 0;
    }
    std::printf("CDragonActionNapalmDef_00454038_TEST FAIL\n");
    return 1;
}