// Standalone behaviour test for CTCHeroStats::GetMoralityMax @ 0x0057a67e
#include <cstdio>

struct CTCHeroStats;

// Fixture: helper returns a controlled pointer; then two chained derefs.
static char g_inner[0x100];   // holds the final field at +0xD0
static char g_mid[0x100];     // holds pointer-to-inner at +0xDC

extern "C" {}

// __fastcall stub matching the modeled helper signature.
void * __fastcall CTCHeroStats_GetHelper(CTCHeroStats *self)
{
    (void)self;
    return (void *)g_mid;
}

long __fastcall CTCHeroStats__GetMoralityMax(CTCHeroStats *self)
{
    char *p = (char *)CTCHeroStats_GetHelper(self);
    char *q = *(char **)(p + 0xDC);
    return *(long *)(q + 0xD0);
}

int main()
{
    // Wire fixture: g_mid+0xDC -> g_inner ; g_inner+0xD0 = 777
    *(char **)(g_mid + 0xDC) = g_inner;
    *(long *)(g_inner + 0xD0) = 777;

    CTCHeroStats *fake = (CTCHeroStats *)0x1; // unused by stub
    long v = CTCHeroStats__GetMoralityMax(fake);

    if (v == 777)
        printf("CTCHEROSTATS_0057a67e_TEST PASS\n");
    else
        printf("FAIL got %ld\n", v);
    return 0;
}