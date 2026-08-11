#include <cstdio>

struct CAnimationEntry {
    unsigned long a;
    unsigned long b;
    unsigned long c;
    ~CAnimationEntry();
};

struct AnimPair {
    unsigned long first;
    CAnimationEntry second;
};

struct AnimAlloc { char dummy; };

static int g_destroyCount = 0;

void __fastcall DestroyPair(AnimPair* self);

void __fastcall Destroy_range(AnimPair* first, AnimPair* last, AnimAlloc* al);

// provide the helper for the standalone link
void __fastcall DestroyPair(AnimPair* self)
{
    self->first = 0xDEAD;
    ++g_destroyCount;
}

// CAnimationEntry dtor is referenced only by declaration; provide a stub
CAnimationEntry::~CAnimationEntry() {}

int main()
{
    AnimPair arr[4];
    for (int i = 0; i < 4; ++i) arr[i].first = (unsigned long)(i + 1);

    AnimAlloc al;
    Destroy_range(arr, arr + 4, &al);

    bool ok = (g_destroyCount == 4);
    for (int i = 0; i < 4; ++i) if (arr[i].first != 0xDEAD) ok = false;

    // empty-range case
    g_destroyCount = 0;
    Destroy_range(arr, arr, &al);
    if (g_destroyCount != 0) ok = false;

    if (sizeof(AnimPair) != 16) ok = false;

    if (ok) printf("DESTROY_RANGE_OK\n");
    else printf("DESTROY_RANGE_FAIL count=%d\n", g_destroyCount);
    return ok ? 0 : 1;
}