#include <cstdio>
struct C3DVector { float x, y, z; };
static int g_called = 0;
static void* g_navArg = 0;
static void* g_destArg = 0;
static bool g_ret = true;
// Model of the retail vtable slot 6 callee: takes (nav, dest)
static bool __fastcall Check(void* nav, int /*edx*/, void* dest){ g_called++; g_navArg=nav; g_destArg=dest; return g_ret; }
struct Helper { void** vptr; };
struct Nav { void* pad0; Helper* helper; };
// Behavioural model of IsNewDestinationGoingToResetNavigation:
//   loads this->helper (offset 4), reads its vtable slot 6, calls slot(this, dest); ignores float.
static bool Model(Nav* self, C3DVector* dest, float /*f*/){
  Helper* h = self->helper;
  void** vt = h->vptr;
  typedef bool (__fastcall *Slot)(void*, int, void*);
  Slot s = (Slot)vt[6];
  return s(self, 0, dest);
}
int main(){
  static void* vtbl[8];
  for (int i=0;i<8;i++) vtbl[i]=0;
  vtbl[6] = (void*)&Check;
  Helper h; h.vptr = vtbl;
  Nav nav; nav.pad0 = 0; nav.helper = &h;
  C3DVector d = {1,2,3};
  // branch 1: callee returns true
  g_ret = true; g_called = 0;
  bool r1 = Model(&nav, &d, 5.0f);
  bool ok1 = r1 && g_called==1 && g_navArg==(void*)&nav && g_destArg==(void*)&d;
  // branch 2: callee returns false (result forwarded verbatim)
  g_ret = false; g_called = 0;
  bool r2 = Model(&nav, &d, 9.0f);
  bool ok2 = (!r2) && g_called==1;
  bool ok = ok1 && ok2;
  printf(ok?"OK_00c46a30\n":"FAIL\n");
  return ok?0:1;
}