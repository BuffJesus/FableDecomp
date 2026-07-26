struct CUserProfileManager {
  char pad0[0xc];
  char flagC;      /* +0xc */
  char pad2[0x74-0xd];
  int  fld74;     /* +0x74 */
  char pad3[0x128-0x78];
  int  fld128;    /* +0x128 */
};
extern void __fastcall UPM_helperA(CUserProfileManager* self);
extern void __fastcall UPM_helperB(CUserProfileManager* self);
void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int edx_ignore, int arg1, char arg2)
{
  (void)edx_ignore;
  if (self->flagC)
    self->fld128 = arg1;
  self->fld74 = arg1;
  UPM_helperA(self);
  if (arg2)
    UPM_helperB(self);
}