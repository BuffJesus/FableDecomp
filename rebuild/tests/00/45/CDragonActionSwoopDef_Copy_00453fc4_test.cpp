#include <cstdio>
struct CDragonActionSwoopDef { int tag; };
static int g_calls = 0;
static CDragonActionSwoopDef* g_last_self = 0;
static const CDragonActionSwoopDef* g_last_other = 0;
void __fastcall CDragonActionSwoopDef_Copy_impl(CDragonActionSwoopDef* self, const CDragonActionSwoopDef* other)
{
    g_calls++;
    g_last_self = self;
    g_last_other = other;
}
void __fastcall CDragonActionSwoopDef_Copy(CDragonActionSwoopDef* self, const CDragonActionSwoopDef* other)
{
    CDragonActionSwoopDef_Copy_impl(self, other);
}
int main()
{
    CDragonActionSwoopDef a; a.tag = 1;
    CDragonActionSwoopDef b; b.tag = 2;
    CDragonActionSwoopDef_Copy(&a, &b);
    if (g_calls == 1 && g_last_self == &a && g_last_other == &b) {
        std::printf("CDragonActionSwoopDef_00453fc4_TEST PASS\n");
        return 0;
    }
    std::printf("CDragonActionSwoopDef_00453fc4_TEST FAIL\n");
    return 1;
}