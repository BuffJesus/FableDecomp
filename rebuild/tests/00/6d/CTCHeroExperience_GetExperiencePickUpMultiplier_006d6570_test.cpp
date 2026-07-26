#include <cstdio>
struct COther { float m64pad[0x64/4]; float f64; };
struct CTCHeroExperience {
  char pad30[0x30];
  float f30;
  char pad34[0x54-0x34];
  COther* p54;
};
long GetMult(const CTCHeroExperience* self) {
  long v = (long)(self->f30 / self->p54->f64 + 0.5);
  if (v < 1) v = 1;
  return v;
}
int main() {
  COther o; o.f64 = 2.0f;
  CTCHeroExperience h; h.f30 = 10.0f; h.p54 = &o;
  long r = GetMult(&h);
  if (r != 5) { std::printf("FAIL got %ld\n", r); return 1; }
  o.f64 = 100.0f; h.f30 = 1.0f;
  r = GetMult(&h);
  if (r != 1) { std::printf("FAIL clamp got %ld\n", r); return 1; }
  std::printf("CTCHeroExperience_006d6570_TEST PASS\n");
  return 0;
}