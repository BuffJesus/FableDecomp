#include <cstdio>

struct CTexMgr {
  int released;
  void ReleaseTexture(void* t) { released = (t != 0) ? 1 : 0; }
};
struct CGlobal { char pad[0x6b0]; CTexMgr* mgr; };
struct CLandscapeBackgroundPatch { char pad[0xb8]; void* tex; };

static CTexMgr g_mgrObj;
static CGlobal g_globalObj;
CGlobal* g_obj = &g_globalObj;

void CLandscapeBackgroundPatch_ClearTextures(CLandscapeBackgroundPatch* self)
{
  void* tex = self->tex;
  if (tex) {
    g_obj->mgr->ReleaseTexture(tex);
    self->tex = 0;
  }
}

int main() {
  g_globalObj.mgr = &g_mgrObj;
  CLandscapeBackgroundPatch p;
  int dummy = 5;
  p.tex = &dummy;
  g_mgrObj.released = 0;
  CLandscapeBackgroundPatch_ClearTextures(&p);
  if (p.tex != 0) { std::printf("FAIL tex not cleared\n"); return 1; }
  if (g_mgrObj.released != 1) { std::printf("FAIL not released\n"); return 1; }
  CLandscapeBackgroundPatch p2;
  p2.tex = 0;
  g_mgrObj.released = 99;
  CLandscapeBackgroundPatch_ClearTextures(&p2);
  if (g_mgrObj.released != 99) { std::printf("FAIL released on null\n"); return 1; }
  std::printf("CLandscapeBackgroundPatch_00be5c30_TEST PASS\n");
  return 0;
}