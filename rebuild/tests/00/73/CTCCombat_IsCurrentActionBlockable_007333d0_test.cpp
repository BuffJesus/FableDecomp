#include <cstdio>
struct CInner { char pad[0x100]; void* p100; };
struct CVtbl { char pad48[0x48]; unsigned char (__fastcall *m)(void*); };
struct CTCCombat { int f0,f4,f8,fC; char pad[0x24-0x10]; CInner* f24; };

static bool checkframe(int v)
{
  return v == 0 || v == (int)0xfffffcf7;
}

bool __fastcall CTCCombat_IsCurrentActionBlockable(CTCCombat* self)
{
  void* obj = self->f24->p100;
  if (obj != 0)
  {
    CVtbl** vt = (CVtbl**)obj;
    if (!(**vt).m(obj))
      return checkframe(self->fC);
  }
  return false;
}

static unsigned char __fastcall retFalse(void* p){ (void)p; return 0; }
static unsigned char __fastcall retTrue(void* p){ (void)p; return 1; }

int main(){
  CInner in0; in0.p100 = 0;
  CTCCombat c0; c0.fC = 5; c0.f24 = &in0;
  if (CTCCombat_IsCurrentActionBlockable(&c0) != false){ std::printf("FAIL1\n"); return 1; }

  CVtbl vtT; vtT.m = retTrue; CVtbl* pvtT = &vtT;
  CInner inT; inT.p100 = &pvtT;
  CTCCombat cT; cT.fC = 0; cT.f24 = &inT;
  if (CTCCombat_IsCurrentActionBlockable(&cT) != false){ std::printf("FAIL2\n"); return 1; }

  CVtbl vtF; vtF.m = retFalse; CVtbl* pvtF = &vtF;
  CInner inF; inF.p100 = &pvtF;
  CTCCombat cA; cA.fC = 0; cA.f24 = &inF;
  if (CTCCombat_IsCurrentActionBlockable(&cA) != true){ std::printf("FAIL3\n"); return 1; }

  CTCCombat cB; cB.fC = (int)0xfffffcf7; cB.f24 = &inF;
  if (CTCCombat_IsCurrentActionBlockable(&cB) != true){ std::printf("FAIL4\n"); return 1; }

  CTCCombat cC; cC.fC = 7; cC.f24 = &inF;
  if (CTCCombat_IsCurrentActionBlockable(&cC) != false){ std::printf("FAIL5\n"); return 1; }

  std::printf("CTCCombat_007333d0_TEST PASS\n");
  return 0;
}