#include <cstdio>
struct C3DVector { float x,y,z; };
struct Nav;
struct Inner {
  int calls; Nav* gotSelf; C3DVector* gotV; bool ret;
};
struct Nav { void* f0; Inner* f4; };
static Inner g_in;
static bool model(Nav* self, C3DVector* dest){
  Inner* in = self->f4;
  in->calls++; in->gotSelf=self; in->gotV=dest;
  return in->ret;
}
int main(){
  g_in.calls=0; g_in.ret=true;
  Nav nav; nav.f4=&g_in;
  C3DVector v={1,2,3};
  bool r = model(&nav, &v);
  int ok=1;
  if(!r) ok=0;
  if(g_in.calls!=1) ok=0;
  if(g_in.gotSelf!=&nav) ok=0;
  if(g_in.gotV!=&v) ok=0;
  g_in.ret=false; g_in.calls=0;
  bool r2 = model(&nav,&v);
  if(r2) ok=0;
  if(g_in.calls!=1) ok=0;
  if(ok) printf("OK_00c2ae70\n"); else printf("FAIL\n");
  return ok?0:1;
}