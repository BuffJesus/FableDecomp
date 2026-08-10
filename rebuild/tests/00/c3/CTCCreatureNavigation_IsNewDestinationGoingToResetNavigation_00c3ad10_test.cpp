#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct IHelper {
  virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest) = 0;
};
struct CTCCreatureNavigation {
  int f0;
  IHelper* f4;
};
static bool g_expected;
struct HelperImpl : IHelper {
  CTCCreatureNavigation* seenNav;
  C3DVector* seenDest;
  bool v7(CTCCreatureNavigation* nav, C3DVector* dest) {
    seenNav = nav; seenDest = dest; return g_expected;
  }
};
static bool ModelLeaf(CTCCreatureNavigation* self, C3DVector* dest, float f) {
  (void)f;
  return self->f4->v7(self, dest);
}
int main() {
  HelperImpl h; h.seenNav = 0; h.seenDest = 0;
  CTCCreatureNavigation nav; nav.f0 = 0; nav.f4 = &h;
  C3DVector d; d.x = 1; d.y = 2; d.z = 3;
  g_expected = true;
  bool r1 = ModelLeaf(&nav, &d, 5.0f);
  bool okargs = (h.seenNav == &nav) && (h.seenDest == &d);
  g_expected = false;
  bool r2 = ModelLeaf(&nav, &d, 9.0f);
  if (r1 == true && r2 == false && okargs) {
    printf("OK_00c3ad10\n");
    return 0;
  }
  printf("FAIL\n");
  return 1;
}