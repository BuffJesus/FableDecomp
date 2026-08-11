#include <stdio.h>

struct AnimPair {
    unsigned long key;
    unsigned long val;
};
struct AnimAlloc { char dummy; };

void __fastcall AnimPair_Destroy(AnimPair* self);
void __fastcall Destroy_range(AnimPair* first, AnimPair* last, AnimAlloc* al);

static int g_count = 0;
static AnimPair* g_seen[16];

void __fastcall AnimPair_Destroy(AnimPair* self)
{
    if (g_count < 16) g_seen[g_count] = self;
    ++g_count;
}

int main()
{
    AnimPair arr[5];
    for (int i = 0; i < 5; ++i) { arr[i].key = (unsigned long)i; arr[i].val = 0; }
    AnimAlloc al;

    Destroy_range(arr, arr + 5, &al);

    int ok = (g_count == 5);
    for (int i = 0; i < 5 && ok; ++i)
        if (g_seen[i] != &arr[i]) ok = 0;

    // empty range: no calls
    g_count = 0;
    Destroy_range(arr, arr, &al);
    if (g_count != 0) ok = 0;

    if (ok) printf("DESTROY_RANGE_OK\n");
    else    printf("DESTROY_RANGE_FAIL count=%d\n", g_count);
    return ok ? 0 : 1;
}