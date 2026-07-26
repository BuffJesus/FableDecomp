struct CUserProfileManager {
  char pad0[0xc];
  char m_flagC;    /* +0xc */
  char pad1[0x8];
  char m_flag15;   /* +0x15 */
  char pad2[0xbf];
  char m_flagD5;   /* +0xd5 */
};
extern void __fastcall CUserProfileManager_HelperA(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_HelperB(CUserProfileManager* self);
void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char resetting, char reload)
{
  (void)edx;
  if (self->m_flagC) {
    self->m_flagD5 = resetting;
  }
  self->m_flag15 = resetting;
  CUserProfileManager_HelperA(self);
  if (reload) {
    CUserProfileManager_HelperB(self);
  }
}