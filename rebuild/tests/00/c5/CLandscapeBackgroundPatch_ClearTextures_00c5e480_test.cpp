#include <cstdio>

struct CLandscapeBackgroundPatch {
  void* vtable;
  void* tex;
};

static int g_released = 0;
static void __stdcall ReleaseImpl(void* t) { g_released = (t != 0) ? 1 : 0; }
void (__stdcall *g_ReleaseTex)(void* t) = &ReleaseImpl;

void __fastcall CLandscapeBackgroundPatch_ClearTextures(CLandscapeBackgroundPatch* self)
{
  void* tex = self->tex;
  self->vtable = (void*)0x12b90ec;
  if (tex) {
    g_ReleaseTex(tex);
    self->tex = 0;
  }
}

int main() {
  CLandscapeBackgroundPatch p;
  int dummy = 5;
  p.vtable = 0;
  p.tex = &dummy;
  g_released = 0;
  CLandscapeBackgroundPatch_ClearTextures(&p);
  if (p.vtable != (void*)0x12b90ec) { std::printf("FAIL vtable not set\n"); return 1; }
  if (p.tex != 0) { std::printf("FAIL tex not cleared\n"); return 1; }
  if (g_released != 1) { std::printf("FAIL not released\n"); return 1; }

  CLandscapeBackgroundPatch p2;
  p2.vtable = 0;
  p2.tex = 0;
  g_released = 99;
  CLandscapeBackgroundPatch_ClearTextures(&p2);
  if (p2.vtable != (void*)0x12b90ec) { std::printf("FAIL vtable not set null\n"); return 1; }
  if (g_released != 99) { std::printf("FAIL released on null\n"); return 1; }

  std::printf("CLandscapeBackgroundPatch_00c5e480_TEST PASS\n");
  return 0;
}