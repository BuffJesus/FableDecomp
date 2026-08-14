#include <cstdio>

struct CAnimationEntry { unsigned long a; };
struct Alloc { char dummy; };
struct Pair { unsigned long first; CAnimationEntry second; };

static int g_count = 0;
static Pair* g_seen[64];

void __fastcall destroy_pair(Pair* p, Alloc* al)
{
    (void)al;
    g_seen[g_count++] = p;
}

void __fastcall Destroy_range(Pair* first, Pair* last, Alloc* al)
{
    for (; first != last; ++first)
        destroy_pair(first, al);
}

int main()
{
    Pair arr[5];
    Alloc a;
    Destroy_range(arr, arr + 5, &a);

    if (g_count != 5) { printf("FAIL count %d\n", g_count); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_seen[i] != &arr[i]) { printf("FAIL order %d\n", i); return 1; }

    g_count = 0;
    Destroy_range(arr, arr, &a);
    if (g_count != 0) { printf("FAIL empty %d\n", g_count); return 1; }

    printf("DESTROY_RANGE_OK\n");
    return 0;
}