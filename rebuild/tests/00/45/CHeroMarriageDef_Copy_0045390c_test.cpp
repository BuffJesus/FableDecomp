#include <cstdio>

struct CHeroMarriageDef { int tag; };

static int g_calls = 0;
static CHeroMarriageDef* g_self = 0;
static void* g_other = 0;

void __fastcall CHeroMarriageDef_CopyImpl(CHeroMarriageDef* self, void* other)
{
    g_calls++;
    g_self = self;
    g_other = other;
}

void __fastcall CHeroMarriageDef_Copy(CHeroMarriageDef* self, void* other)
{
    CHeroMarriageDef_CopyImpl(self, other);
}

int main()
{
    CHeroMarriageDef d; d.tag = 7;
    int marker = 0x1234;
    CHeroMarriageDef_Copy(&d, &marker);
    if (g_calls == 1 && g_self == &d && g_other == &marker) {
        std::printf("CHeroMarriageDef_0045390c_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}