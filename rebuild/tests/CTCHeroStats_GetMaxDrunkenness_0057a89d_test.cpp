#include <cstdio>

struct CTCHeroStatsInner { char pad[0xa4]; float maxDrunkenness; };
struct CTCHeroStats { char pad[0xdc]; CTCHeroStatsInner* inner; };

static CTCHeroStats* g_owner = 0;

CTCHeroStats* __fastcall CTCHeroStats_GetOwner(CTCHeroStats* self)
{
    (void)self;
    return g_owner;
}

float __fastcall CTCHeroStats_GetMaxDrunkenness(CTCHeroStats* self)
{
    CTCHeroStats* p = CTCHeroStats_GetOwner(self);
    return p->inner->maxDrunkenness;
}

int main()
{
    CTCHeroStatsInner inner;
    inner.maxDrunkenness = 42.5f;
    CTCHeroStats owner;
    owner.inner = &inner;
    g_owner = &owner;

    CTCHeroStats dummy;
    float r = CTCHeroStats_GetMaxDrunkenness(&dummy);
    if (r == 42.5f) {
        std::printf("CTCHeroStats_0057a89d_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}