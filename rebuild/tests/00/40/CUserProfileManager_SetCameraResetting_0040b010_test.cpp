#include <cstdio>

struct CUserProfileManager {
  char pad0[0xc];
  char gate;
  char pad1[2];
  char flagF;
  char pad2[0xbf];
  char flagCF;
};

static int g_sub1=0, g_sub2=0;
void __fastcall CUPM_sub1(CUserProfileManager* self){ (void)self; g_sub1++; }
void __fastcall CUPM_sub2(CUserProfileManager* self){ (void)self; g_sub2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, char a, char b)
{
  if (self->gate)
    self->flagCF = a;
  self->flagF = a;
  CUPM_sub1(self);
  if (b)
    CUPM_sub2(self);
}

int main(){
  CUserProfileManager o;
  o.gate=1; o.flagF=0; o.flagCF=0;
  g_sub1=g_sub2=0;
  CUserProfileManager_SetCameraResetting(&o, 7, 1);
  if(o.flagCF!=7){ std::printf("FAIL flagCF\n"); return 1; }
  if(o.flagF!=7){ std::printf("FAIL flagF\n"); return 1; }
  if(g_sub1!=1){ std::printf("FAIL sub1\n"); return 1; }
  if(g_sub2!=1){ std::printf("FAIL sub2\n"); return 1; }

  o.gate=0; o.flagF=0; o.flagCF=3;
  g_sub1=g_sub2=0;
  CUserProfileManager_SetCameraResetting(&o, 9, 0);
  if(o.flagCF!=3){ std::printf("FAIL flagCF2\n"); return 1; }
  if(o.flagF!=9){ std::printf("FAIL flagF2\n"); return 1; }
  if(g_sub1!=1){ std::printf("FAIL sub1b\n"); return 1; }
  if(g_sub2!=0){ std::printf("FAIL sub2b\n"); return 1; }

  std::printf("CUserProfileManager_0040b010_TEST PASS\n");
  return 0;
}