#pragma optimize("s",on)
struct CTCHeroStats {
  int m00[10];
  int m28;
};
extern "C" int __fastcall CTCHeroStats_GetMax(CTCHeroStats* self);
extern "C" int __fastcall CTCHeroStats_GetVal(CTCHeroStats* self);

float __fastcall CTCHeroStats_GetMoralityPercentage(CTCHeroStats* self)
{
  int denom = CTCHeroStats_GetMax(self) * 2;
  int num = CTCHeroStats_GetVal(self) + self->m28;
  return (float)num / (float)denom;
}