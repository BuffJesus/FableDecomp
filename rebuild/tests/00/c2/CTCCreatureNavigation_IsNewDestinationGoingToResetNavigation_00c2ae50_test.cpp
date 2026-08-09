#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Inner {
  struct InnerVtbl {
    void* s0; void* s1; void* s2; void* s3; void* s4; void* s5; void* s6;
    bool (__stdcall* isReset)(CTCCreatureNavigation* self, C3DVector* dest);
  };
  InnerVtbl* vtbl;
};
struct CTCCreatureNavigation {
  void* field0;
  Inner* field4;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest, float);

static CTCCreatureNavigation* g_expectedSelf;
static C3DVector* g_expectedDest;
static bool g_ret;
static bool __stdcall model_isReset(CTCCreatureNavigation* self, C3DVector* dest) {
  if (self != g_expectedSelf) { printf("BAD self\n"); return false; }
  if (dest != g_expectedDest) { printf("BAD dest\n"); return false; }
  return g_ret;
}

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest, float)
{
  Inner* inner = self->field4;
  return inner->vtbl->isReset(self, dest);
}

int main() {
  Inner::InnerVtbl vt; vt.isReset = &model_isReset;
  Inner inner; inner.vtbl = &vt;
  CTCCreatureNavigation nav; nav.field0 = 0; nav.field4 = &inner;
  C3DVector d = {1.0f, 2.0f, 3.0f};
  g_expectedSelf = &nav; g_expectedDest = &d;
  int fails = 0;
  g_ret = true;
  if (IsNewDestinationGoingToResetNavigation(&nav, 0, &d, 5.0f) != true) fails++;
  g_ret = false;
  if (IsNewDestinationGoingToResetNavigation(&nav, 0, &d, 5.0f) != false) fails++;
  if (fails == 0) printf("OK_00c2ae50\n"); else printf("FAIL %d\n", fails);
  return fails;
}