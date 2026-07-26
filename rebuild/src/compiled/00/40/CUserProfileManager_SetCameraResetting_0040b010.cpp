struct CUserProfileManager {
  char pad0[0xc];
  char gate;   /* +0xc */
  char pad1[2];
  char flagF;  /* +0xf */
  char pad2[0xbf];
  char flagCF; /* +0xcf */
  void CUserProfileManager_SetCameraResetting(char a, char b);
  void sub1();
  void sub2();
};

void CUserProfileManager::CUserProfileManager_SetCameraResetting(char a, char b)
{
  if (this->gate)
    this->flagCF = a;
  this->flagF = a;
  this->sub1();
  if (b)
    this->sub2();
}