#include <cstdio>

static int g_freed = 0;
static int g_deleted = 0;
static int g_inner = 0;

void my_free(void* p){ if(p) g_freed++; }
void my_delete(void* p){ if(p) g_deleted++; }

struct CInner { int x; };
void __fastcall CInner_dtor(CInner* p){ (void)p; g_inner++; }

struct CLandscapeBackgroundPatch {
  int f0;
  int f4;
  void* f8;
  CInner inner;
  void* CLandscapeBackgroundPatch_ClearTextures(int flag);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_ClearTextures(int flag)
{
  if (this->f8) {
    my_free(this->f8);
    this->f8 = 0;
  }
  this->f4 = 0;
  CInner_dtor(&this->inner);
  if (flag & 1) {
    my_delete(this);
  }
  return this;
}

int main(){
  CLandscapeBackgroundPatch obj;
  obj.f0 = 1; obj.f4 = 99; static int dummy; obj.f8 = &dummy; obj.inner.x = 7;
  void* r = obj.CLandscapeBackgroundPatch_ClearTextures(0);
  if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
  if (obj.f8 != 0) { std::printf("FAIL f8\n"); return 1; }
  if (obj.f4 != 0) { std::printf("FAIL f4\n"); return 1; }
  if (g_freed != 1) { std::printf("FAIL freed\n"); return 1; }
  if (g_inner != 1) { std::printf("FAIL inner\n"); return 1; }
  if (g_deleted != 0) { std::printf("FAIL deleted0\n"); return 1; }
  CLandscapeBackgroundPatch obj2;
  obj2.f8 = 0; obj2.f4 = 5; obj2.inner.x = 0;
  obj2.CLandscapeBackgroundPatch_ClearTextures(1);
  if (g_deleted != 1) { std::printf("FAIL deleted1\n"); return 1; }
  if (g_freed != 1) { std::printf("FAIL freed2\n"); return 1; }
  std::printf("CLandscapeBackgroundPatch_009d3100_TEST PASS\n");
  return 0;
}