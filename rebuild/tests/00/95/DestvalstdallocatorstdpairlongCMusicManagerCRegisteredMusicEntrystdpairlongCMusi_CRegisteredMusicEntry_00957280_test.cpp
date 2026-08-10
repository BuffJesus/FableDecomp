#include <cstdio>
struct Sub { void* p; };
struct Obj { char pad[0x114]; Sub sub; };
static int g_sub=0; static int g_base=0; static Sub* g_subptr=0; static Obj* g_baseptr=0;
void sub_helper(Sub* s){ g_sub++; g_subptr=s; }
void base_dtor(Obj* o){ g_base++; g_baseptr=o; }
void model(Obj* self){ sub_helper(&self->sub); base_dtor(self); }
int main(){
  Obj o; 
  model(&o);
  int ok = (g_sub==1) && (g_base==1) && (g_subptr==&o.sub) && (g_baseptr==&o) && ((char*)&o.sub - (char*)&o == 0x114);
  if(ok) printf("OK_00957280\n"); else printf("FAIL\n");
  return ok?0:1;
}