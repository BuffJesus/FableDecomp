#include <cstdio>

struct pair_unsigned_long_CAnimationEntry {
    unsigned long first;
    void* second;
};

struct MyAlloc {
    char dummy;
};

static int g_destroyed = 0;
static unsigned long g_lastKey = 0xffffffff;

// element destructor stub (retail 0x4d799a): records each visited element
void __fastcall destroy_pair_unsigned_long_CAnimationEntry(
    pair_unsigned_long_CAnimationEntry* self)
{
    g_destroyed++;
    g_lastKey = self->first;
    self->second = 0;
}

void __fastcall Destroy_range_pair_unsigned_long_CAnimationEntry(
    pair_unsigned_long_CAnimationEntry* first,
    pair_unsigned_long_CAnimationEntry* last,
    MyAlloc* al)
{
    for (; first != last; ++first)
        destroy_pair_unsigned_long_CAnimationEntry(first);
    (void)al;
}

int main()
{
    pair_unsigned_long_CAnimationEntry arr[5];
    for (int i = 0; i < 5; ++i) { arr[i].first = (unsigned long)(i * 10); arr[i].second = (void*)1; }
    MyAlloc a;

    // full range
    g_destroyed = 0; g_lastKey = 0xffffffff;
    Destroy_range_pair_unsigned_long_CAnimationEntry(&arr[0], &arr[5], &a);
    bool ok1 = (g_destroyed == 5) && (g_lastKey == 40) && (arr[2].second == 0);

    // empty range (first == last): no calls
    g_destroyed = 0;
    Destroy_range_pair_unsigned_long_CAnimationEntry(&arr[2], &arr[2], &a);
    bool ok2 = (g_destroyed == 0);

    if (ok1 && ok2)
        printf("DESTROY_RANGE_OK destroyed=5 empty=0\n");
    else
        printf("FAIL d=%d k=%lu\n", g_destroyed, g_lastKey);
    return (ok1 && ok2) ? 0 : 1;
}