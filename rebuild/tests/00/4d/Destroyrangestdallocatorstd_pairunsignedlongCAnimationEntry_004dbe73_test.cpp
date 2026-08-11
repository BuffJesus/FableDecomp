#include <stdio.h>

struct CAnimEntryPair;
struct AllocDummy {};

static int g_calls = 0;
static char* g_last = 0;

extern "C" void __fastcall Pair_Dtor(CAnimEntryPair* self)
{
    ++g_calls;
    g_last = (char*)self;
}

void __fastcall _Destroy_range_pair(CAnimEntryPair* first, CAnimEntryPair* last, AllocDummy* al);

int main()
{
    char buf[64];
    AllocDummy al;
    // 3 elements of stride 0xc => 36 bytes
    CAnimEntryPair* first = (CAnimEntryPair*)&buf[0];
    CAnimEntryPair* last  = (CAnimEntryPair*)&buf[36];

    g_calls = 0;
    _Destroy_range_pair(first, last, &al);
    if (g_calls != 3) { printf("FAIL count=%d\n", g_calls); return 1; }
    // last processed element should be at buf+24
    if (g_last != &buf[24]) { printf("FAIL last\n"); return 1; }

    // empty range => no calls
    g_calls = 0;
    _Destroy_range_pair(first, first, &al);
    if (g_calls != 0) { printf("FAIL empty=%d\n", g_calls); return 1; }

    printf("DESTROY_RANGE_OK\n");
    return 0;
}