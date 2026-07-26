#include <cstdio>
struct S { int v[64]; };
static bool g_dtor=false; static bool g_del=false;
void dtor(void* s){ g_dtor=true; }
void del(void* p){ g_del=true; }
void* vec_dtor(void* self, char flags){
  *(int*)((char*)self+0x28)=0x1232484;
  *(int*)self=0x1230ba0;
  dtor(self);
  if(flags&1) del(self);
  return self;
}
int main(){
  S s; 
  g_dtor=g_del=false;
  void* r=vec_dtor(&s,1);
  if(r!=&s){ std::printf("FAIL ret\n"); return 1; }
  if(!g_dtor){ std::printf("FAIL dtor\n"); return 1; }
  if(!g_del){ std::printf("FAIL del\n"); return 1; }
  if(s.v[0]!=0x1230ba0){ std::printf("FAIL vt0\n"); return 1; }
  if(s.v[0x28/4]!=0x1232484){ std::printf("FAIL vt28\n"); return 1; }
  g_dtor=g_del=false;
  vec_dtor(&s,0);
  if(g_del){ std::printf("FAIL nodel\n"); return 1; }
  std::printf("CLandscapeBackgroundPatch_0044fe12_TEST PASS\n");
  return 0;
}