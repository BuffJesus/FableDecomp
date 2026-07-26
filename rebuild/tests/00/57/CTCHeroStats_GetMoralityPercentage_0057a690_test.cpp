#include <cstdio>
struct CTCHeroStats {
  int m00[10];
  int m28;
};
static int g_max = 50;
static int g_val = 30;
extern "C" int __fastcall CTCHeroStats_GetMax(CTCHeroStats* self){ (void)self; return g_max; }
extern "C" int __fastcall CTCHeroStats_GetVal(CTCHeroStats* self){ (void)self; return g_val; }

float __fastcall CTCHeroStats_GetMoralityPercentage(CTCHeroStats* self)
{
  int denom = CTCHeroStats_GetMax(self) * 2;
  int num = CTCHeroStats_GetVal(self) + self->m28;
  return (float)num / (float)denom;
}

int main(){
  CTCHeroStats s; s.m28 = 20;
  g_max = 50; g_val = 30;
  float r = CTCHeroStats_GetMoralityPercentage(&s);
  // num = 30+20=50, denom=100 => 0.5
  if (r > 0.4999f && r < 0.5001f) {
    std::printf("CTCHeroStats_0057a690_TEST PASS\n");
    return 0;
  }
  std::printf("FAIL r=%f\n", r);
  return 1;
}