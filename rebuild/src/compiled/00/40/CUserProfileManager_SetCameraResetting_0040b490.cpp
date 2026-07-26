struct CUserProfileManager {
  char pad0[0xc];
  char m_gate;
  char pad2[0x6c-0xd];
  char m_flag6c;
  char pad3[0x134-0x6d];
  char m_flag134;
};
extern void __fastcall CUserProfileManager_OnA(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_OnB(CUserProfileManager* self);
void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char a, char b) {
  (void)edx;
  if (self->m_gate)
    self->m_flag134 = a;
  self->m_flag6c = a;
  CUserProfileManager_OnA(self);
  if (b)
    CUserProfileManager_OnB(self);
}