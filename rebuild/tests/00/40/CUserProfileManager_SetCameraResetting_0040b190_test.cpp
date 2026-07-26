#include <cstdio>
struct CUserProfileManager {
  char pad0[0xc];
  char m_flagC;
  char pad1[0x8];
  char m_flag15;
  char pad2[0xbf];
  char m_flagD5;
};
static int g_a=0, g_b=0;
void CUserProfileManager_HelperA(CUserProfileManager* self){ (void)self; g_a++; }
void CUserProfileManager_HelperB(CUserProfileManager* self){ (void)self; g_b++; }
void CUserProfileManager_SetCameraResetting(CUserProfileManager* self, char resetting, char reload)
{
  if (self->m_flagC) { self->m_flagD5 = resetting; }
  self->m_flag15 = resetting;
  CUserProfileManager_HelperA(self);
  if (reload) { CUserProfileManager_HelperB(self); }
}
int main(){
  CUserProfileManager o; o.m_flagC=1; o.m_flagD5=0; o.m_flag15=0;
  g_a=g_b=0;
  CUserProfileManager_SetCameraResetting(&o, 7, 1);
  if (o.m_flagD5!=7) { std::printf("FAIL flagD5\n"); return 1; }
  if (o.m_flag15!=7) { std::printf("FAIL flag15\n"); return 1; }
  if (g_a!=1 || g_b!=1) { std::printf("FAIL calls\n"); return 1; }
  CUserProfileManager o2; o2.m_flagC=0; o2.m_flagD5=3; o2.m_flag15=0;
  g_a=g_b=0;
  CUserProfileManager_SetCameraResetting(&o2, 9, 0);
  if (o2.m_flagD5!=3) { std::printf("FAIL flagD5 skip\n"); return 1; }
  if (o2.m_flag15!=9) { std::printf("FAIL flag15 b\n"); return 1; }
  if (g_a!=1 || g_b!=0) { std::printf("FAIL calls b\n"); return 1; }
  std::printf("CUserProfileManager_0040b190_TEST PASS\n");
  return 0;
}