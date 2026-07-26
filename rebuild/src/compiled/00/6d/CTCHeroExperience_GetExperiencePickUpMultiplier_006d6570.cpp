struct COther { float m64pad[0x64/4]; float f64; };
struct CTCHeroExperience {
  char pad30[0x30];
  float f30;
  char pad34[0x54-0x34];
  COther* p54;
};
long __fastcall CTCHeroExperience_GetExperiencePickUpMultiplier(const CTCHeroExperience* self) {
  long v = (long)(self->f30 / self->p54->f64 + 0.5);
  if (v < 1) v = 1;
  return v;
}