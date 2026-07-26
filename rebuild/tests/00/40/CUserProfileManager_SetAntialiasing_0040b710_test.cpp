#include <cstdio>
struct CUserProfileManager {
  char pad0[0xc];
  char flagC;
  char pad2[0x74-0xd];
  int  fld74;
  char pad3[0x128-0x78];
  int  fld128;
};
static int g_a=0, g_b=0;
void __fastcall UPM_helperA(CUserProfileManager* self){ (void)self; g_a++; }
void __fastcall UPM_helperB(CUserProfileManager* self){ (void)self; g_b++; }
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
int main(){
  CUserProfileManager o;
  for (int i=0;i<sizeof(o);i++) ((char*)&o)[i]=0;
  o.flagC=1;
  g_a=g_b=0;
  CUserProfileManager_SetAntialiasing(&o, 0, 5, 1);
  if (o.fld128!=5 || o.fld74!=5 || g_a!=1 || g_b!=1){ std::printf("FAIL case1\n"); return 1; }
  o.flagC=0; o.fld128=99; o.fld74=0; g_a=g_b=0;
  CUserProfileManager_SetAntialiasing(&o, 0, 7, 0);
  if (o.fld128!=99 || o.fld74!=7 || g_a!=1 || g_b!=0){ std::printf("FAIL case2\n"); return 1; }
  std::printf("CUserProfileManager_0040b710_TEST PASS\n");
  return 0;
}