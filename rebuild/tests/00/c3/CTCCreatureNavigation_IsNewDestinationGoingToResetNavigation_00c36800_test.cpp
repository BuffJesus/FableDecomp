#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Helper {
  virtual bool m0(CTCCreatureNavigation*, C3DVector*){return false;}
  virtual bool m1(CTCCreatureNavigation*, C3DVector*){return false;}
  virtual bool m2(CTCCreatureNavigation*, C3DVector*){return false;}
  virtual bool m3(CTCCreatureNavigation*, C3DVector*){return false;}
  virtual bool m4(CTCCreatureNavigation*, C3DVector*){return false;}
  virtual bool m5(CTCCreatureNavigation*, C3DVector*){return false;}
  virtual bool m6(CTCCreatureNavigation* self, C3DVector* p);
};
struct CTCCreatureNavigation {
  void* f0;
  Helper* f4;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* v)
  {
    return this->f4->m6(this, v);
  }
};
static CTCCreatureNavigation* g_expect_self;
static C3DVector* g_expect_v;
static bool g_ret;
bool Helper::m6(CTCCreatureNavigation* self, C3DVector* p)
{
  if (self != g_expect_self) { printf("BAD self\n"); return false; }
  if (p != g_expect_v) { printf("BAD v\n"); return false; }
  return g_ret;
}
int main()
{
  Helper h;
  CTCCreatureNavigation nav;
  nav.f0 = 0;
  nav.f4 = &h;
  C3DVector v = {1,2,3};
  g_expect_self = &nav;
  g_expect_v = &v;
  g_ret = true;
  bool r1 = nav.IsNewDestinationGoingToResetNavigation(&v);
  g_ret = false;
  bool r2 = nav.IsNewDestinationGoingToResetNavigation(&v);
  if (r1 == true && r2 == false) { printf("OK_00c36800\n"); return 0; }
  printf("FAIL\n"); return 1;
}