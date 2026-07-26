#include <cstdio>
struct CUserProfileManager {
  char pad0[0xc];
  char m_gate;
  char pad2[0x6c-0xd];
  char m_flag6c;
  char pad3[0x134-0x6d];
  char m_flag134;
};
static int g_a=0, g_b=0;
void __fastcall CUserProfileManager_OnA(CUserProfileManager* self){ (void)self; g_a++; }
void __fastcall CUserProfileManager_OnB(CUserProfileManager* self){ (void)self; g_b++; }
void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char a, char b) {
  (void)edx;
  if (self->m_gate)
    self->m_flag134 = a;
  self->m_flag6c = a;
  CUserProfileManager_OnA(self);
  if (b)
    CUserProfileManager_OnB(self);
}
int main(){
  CUserProfileManager o; for(unsigned i=0;i<sizeof(o);++i) ((char*)&o)[i]=0;
  o.m_gate=1;
  CUserProfileManager_SetCameraResetting(&o,0,7,1);
  if(o.m_flag134!=7){ std::printf("FAIL flag134\n"); return 1; }
  if(o.m_flag6c!=7){ std::printf("FAIL flag6c\n"); return 1; }
  if(g_a!=1||g_b!=1){ std::printf("FAIL calls %d %d\n",g_a,g_b); return 1; }
  g_a=g_b=0; o.m_gate=0; o.m_flag134=0; o.m_flag6c=0;
  CUserProfileManager_SetCameraResetting(&o,0,5,0);
  if(o.m_flag134!=0){ std::printf("FAIL gated\n"); return 1; }
  if(o.m_flag6c!=5){ std::printf("FAIL flag6c2\n"); return 1; }
  if(g_a!=1||g_b!=0){ std::printf("FAIL calls2 %d %d\n",g_a,g_b); return 1; }
  std::printf("CUserProfileManager_0040b490_TEST PASS\n");
  return 0;
}