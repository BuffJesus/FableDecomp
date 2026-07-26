#include <cstdio>

static int g_calls = 0;
static void* g_self = 0;
static void* g_other = 0;

void __fastcall CVillagePeopleDef_CopyImpl(void* self, void* other)
{
    g_calls++;
    g_self = self;
    g_other = other;
}

void __fastcall CVillagePeopleDef_Copy(void* self, void* other)
{
    CVillagePeopleDef_CopyImpl(self, other);
}

int main()
{
    int a = 1, b = 2;
    CVillagePeopleDef_Copy(&a, &b);
    if (g_calls == 1 && g_self == &a && g_other == &b) {
        std::printf("CVillagePeopleDef_0045473a_TEST PASS\n");
        return 0;
    }
    std::printf("CVillagePeopleDef_0045473a_TEST FAIL\n");
    return 1;
}