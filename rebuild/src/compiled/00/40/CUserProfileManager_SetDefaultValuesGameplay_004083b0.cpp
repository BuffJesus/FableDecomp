struct CUserProfileManager {
  char pad0[0x24];
  float f24;
  float f28;
  float f2c;
  unsigned char b30;
  unsigned char b31;
};

extern void __fastcall CUserProfileManager_Helper(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetDefaultValuesGameplay(CUserProfileManager* self)
{
  self->f24 = 0.8f;
  self->f28 = 0.6f;
  self->f2c = 0.9f;
  self->b31 = 0;
  self->b30 = 0;
  CUserProfileManager_Helper(self);
}