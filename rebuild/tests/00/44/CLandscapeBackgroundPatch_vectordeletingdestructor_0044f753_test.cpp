#include <cstdio>
// Behavioural model of the two logical paths encoded in 0x0044f753.
struct CLandscapeBackgroundPatch { void* vptr; };
static int g_dtor=0, g_del=0, g_ctor=0;
static void dt(CLandscapeBackgroundPatch* p){ (void)p; g_dtor++; }
static void od(void* p){ (void)p; g_del++; }
static void ct(void* p){ (void)p; g_ctor++; }
static char g_buf[48];
static void* nw(unsigned int n){ (void)n; return g_buf; }

// path 1: scalar deleting destructor
static void* del_dtor(CLandscapeBackgroundPatch* self, unsigned char flags){
  self->vptr = (void*)0x1230ba0;
  dt(self);
  if (flags & 1) od(self);
  return self;
}
// path 2: new-expression factory
static void* create(){
  void* p = nw(0x30);
  if (p) { ct(p); return p; }
  return 0;
}

int main(){
  CLandscapeBackgroundPatch o; o.vptr = 0;
  void* r = del_dtor(&o, 0);
  if (r != &o || g_dtor != 1 || g_del != 0) { std::printf("FAIL a\n"); return 1; }
  r = del_dtor(&o, 1);
  if (r != &o || g_dtor != 2 || g_del != 1) { std::printf("FAIL b\n"); return 1; }
  void* c = create();
  if (c != g_buf || g_ctor != 1) { std::printf("FAIL c\n"); return 1; }
  std::printf("CLandscapeBackgroundPatch_0044f753_TEST PASS\n");
  return 0;
}