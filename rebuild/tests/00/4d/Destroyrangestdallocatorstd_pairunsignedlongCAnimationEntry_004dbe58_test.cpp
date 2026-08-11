#include <cstdio>

struct CAnimationEntry
{
    unsigned long a;
    unsigned long b;
    unsigned long c;
    unsigned long d;
    unsigned long e;
    void* p;
};

struct pair_ulong_anim
{
    unsigned long first;
    CAnimationEntry second;
    void DestroyElement();
};

struct allocator_pair {};

int g_destroyed = 0;
// provide the out-of-line element destructor thunk the loop calls
void pair_ulong_anim::DestroyElement()
{
    if (second.p) ++g_destroyed;
}

void __fastcall Destroy_range_pair_CAnimationEntry(pair_ulong_anim* first,
                                                   pair_ulong_anim* last,
                                                   allocator_pair* al)
{
    (void)al;
    while (first != last)
    {
        first->DestroyElement();
        ++first;
    }
}

int main()
{
    pair_ulong_anim arr[4];
    for (int i = 0; i < 4; ++i) { arr[i].first = (unsigned long)i; arr[i].second.p = (void*)(long)(i + 1); }
    allocator_pair al;
    g_destroyed = 0;
    // destroy the middle two [1,3)
    Destroy_range_pair_CAnimationEntry(arr + 1, arr + 3, &al);
    int mid = g_destroyed;
    // destroy the whole range
    g_destroyed = 0;
    Destroy_range_pair_CAnimationEntry(arr, arr + 4, &al);
    int all = g_destroyed;
    // empty range is a no-op
    g_destroyed = 0;
    Destroy_range_pair_CAnimationEntry(arr, arr, &al);
    int none = g_destroyed;

    if (mid == 2 && all == 4 && none == 0 && sizeof(pair_ulong_anim) == 28)
        printf("DESTROY_RANGE_OK\n");
    else
        printf("FAIL mid=%d all=%d none=%d sz=%d\n", mid, all, none, (int)sizeof(pair_ulong_anim));
    return (mid == 2 && all == 4 && none == 0 && sizeof(pair_ulong_anim) == 28) ? 0 : 1;
}