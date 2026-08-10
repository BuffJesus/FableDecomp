#include <cstdio>
struct Entry { char pad0[0x10]; void* p; char pad1[0x48-0x14]; };
struct Obj { char pad0[0x20]; Entry* table; };
void* g_called=0;
void __fastcall sub_821850(void* self){ g_called=self; }
void __fastcall ConsVal_ParticleMorphs(Obj* self, int, int index){ sub_821850(self->table[index].p); }
int main(){
  Entry ents[3]; int a=11,b=22,c=33;
  ents[0].p=&a; ents[1].p=&b; ents[2].p=&c;
  Obj o; o.table=ents;
  ConsVal_ParticleMorphs(&o,0,1);
  int ok = (g_called==&b);
  ConsVal_ParticleMorphs(&o,0,2);
  ok = ok && (g_called==&c);
  ConsVal_ParticleMorphs(&o,0,0);
  ok = ok && (g_called==&a);
  if(ok) printf("OK_004fd020\n");
  else printf("BAD %p\n", g_called);
  return ok?0:1;
}