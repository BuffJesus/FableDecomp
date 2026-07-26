#include <cstdio>

struct CHeroTitleDef { int x; };

static int g_called = 0;
static CHeroTitleDef* g_self = 0;
static void* g_src = 0;

void __fastcall CHeroTitleDef_Copy_impl(CHeroTitleDef* self, void* src)
{
    g_called = 1;
    g_self = self;
    g_src = src;
}

void __fastcall CHeroTitleDef_Copy(CHeroTitleDef* self, void* src)
{
    CHeroTitleDef_Copy_impl(self, src);
}

int main()
{
    CHeroTitleDef obj; obj.x = 7;
    int marker = 42;
    CHeroTitleDef_Copy(&obj, &marker);
    if (g_called == 1 && g_self == &obj && g_src == (void*)&marker) {
        std::printf("CHeroTitleDef_00453ce5_TEST PASS\n");
        return 0;
    }
    std::printf("CHeroTitleDef_00453ce5_TEST FAIL\n");
    return 1;
}