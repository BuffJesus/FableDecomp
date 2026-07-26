#include <cstdio>

static int g_calls = 0;
static void* g_self = 0;
static void* g_other = 0;

void __fastcall CContextSensitiveItemDef_CopyImpl(void* self, void* other)
{
    g_calls++;
    g_self = self;
    g_other = other;
}

void __fastcall CContextSensitiveItemDef_Copy(void* self, void* other)
{
    CContextSensitiveItemDef_CopyImpl(self, other);
}

int main()
{
    int a = 1, b = 2;
    CContextSensitiveItemDef_Copy(&a, &b);
    if (g_calls == 1 && g_self == &a && g_other == &b) {
        std::printf("CContextSensitiveItemDef_00453c1b_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}