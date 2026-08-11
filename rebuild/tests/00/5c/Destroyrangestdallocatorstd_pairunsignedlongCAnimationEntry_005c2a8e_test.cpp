#include <cstdio>

struct AnimPair {
    unsigned long first;
    unsigned long second[10];
    void Dtor();
};

struct AllocPair {};

void __fastcall _Destroy_range(AnimPair* first, AnimPair* last, AllocPair* al);

static int g_dtor_calls = 0;

void AnimPair::Dtor() { g_dtor_calls++; }

int main()
{
    AnimPair buf[3];
    for (int i = 0; i < 3; ++i) buf[i].first = (unsigned long)i;

    AllocPair al;
    g_dtor_calls = 0;
    _Destroy_range(&buf[0], &buf[3], &al);

    // empty range -> no calls
    _Destroy_range(&buf[0], &buf[0], &al);

    if (g_dtor_calls == 3 && sizeof(AnimPair) == 0x2c) {
        printf("DESTROYRANGE_OK\n");
        return 0;
    }
    printf("FAIL calls=%d size=%d\n", g_dtor_calls, (int)sizeof(AnimPair));
    return 1;
}