#include <cstdio>
struct CDegradableDef { int v; };
static CDegradableDef* g_last_self = 0;
static const CDegradableDef* g_last_src = 0;
void __fastcall CDegradableDef_Copy_impl(CDegradableDef* self, void* edx, const CDegradableDef* src)
{
    (void)edx;
    g_last_self = self;
    g_last_src = src;
}
void __fastcall CDegradableDef_Copy(CDegradableDef* self, void* edx, const CDegradableDef* src)
{
    CDegradableDef_Copy_impl(self, edx, src);
}
int main()
{
    CDegradableDef a; a.v = 1;
    CDegradableDef b; b.v = 2;
    CDegradableDef_Copy(&a, 0, &b);
    if (g_last_self == &a && g_last_src == &b) {
        std::printf("CDegradableDef_0045442f_TEST PASS\n");
        return 0;
    }
    std::printf("CDegradableDef_0045442f_TEST FAIL\n");
    return 1;
}