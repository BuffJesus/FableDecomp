struct CUserProfileManager {
  char pad0[0xC];
  char m_flagC;       /* +0x0C */
  char pad0d[0x14-0x0D];
  char m_state14;     /* +0x14 */
  char pad15[0xD4-0x15];
  char m_stateD4;     /* +0xD4 */
};

extern void __fastcall CUserProfileManager_Sub1(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_Sub2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int unused, char a, char b)
{
  if (self->m_flagC)
    self->m_stateD4 = a;
  self->m_state14 = a;
  CUserProfileManager_Sub1(self);
  if (b)
    CUserProfileManager_Sub2(self);
}