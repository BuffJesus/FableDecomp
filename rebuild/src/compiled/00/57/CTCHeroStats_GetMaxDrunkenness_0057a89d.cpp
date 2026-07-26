struct CTCHeroStatsInner { char pad[0xa4]; float maxDrunkenness; };
struct CTCHeroStats { char pad[0xdc]; CTCHeroStatsInner* inner; };

extern CTCHeroStats* __fastcall CTCHeroStats_GetOwner(CTCHeroStats* self);

float __fastcall CTCHeroStats_GetMaxDrunkenness(CTCHeroStats* self)
{
    CTCHeroStats* p = CTCHeroStats_GetOwner(self);
    return p->inner->maxDrunkenness;
}