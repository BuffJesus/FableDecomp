#include <stdio.h>

struct Alloc { char dummy; };

struct Pair {
    unsigned long a, b, c, d;
    void destroy(Alloc* al);
};

static int g_destroyed = 0;
static Pair* g_seen[8];
static int g_seenN = 0;

void Pair::destroy(Alloc* al)
{
    (void)al;
    g_seen[g_seenN++] = this;
    g_destroyed++;
}

void __fastcall Destroy_range(Pair* _First, Pair* _Last, Alloc* _Al)
{
    for (; _First != _Last; ++_First)
        _First->destroy(_Al);
}

int main()
{
    Pair arr[4];
    Alloc al;
    Destroy_range(arr, arr + 4, &al);

    if (g_destroyed != 4) { printf("FAIL count %d\n", g_destroyed); return 1; }
    for (int i = 0; i < 4; ++i) {
        if (g_seen[i] != &arr[i]) { printf("FAIL order %d\n", i); return 1; }
    }
    // empty range: no destroys
    g_destroyed = 0; g_seenN = 0;
    Destroy_range(arr, arr, &al);
    if (g_destroyed != 0) { printf("FAIL empty\n"); return 1; }

    printf("DESTROY_RANGE_OK\n");
    return 0;
}