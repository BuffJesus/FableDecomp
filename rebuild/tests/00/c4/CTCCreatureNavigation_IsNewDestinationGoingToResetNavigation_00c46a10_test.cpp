#include <cstdio>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
struct Inner;
struct InnerVtbl { 
  char pad[0x1c];
  bool (__fastcall *slot7)(Inner* self, int edx, CTCCreatureNavigation* owner, C3DVector* dest);
};
struct Inner { InnerVtbl* vtbl; };
struct CTCCreatureNavigation { void* f0; Inner* f4; };

static CTCCreatureNavigation* g_owner;
static Inner* g_inner;
static C3DVector* g_dest;
static bool g_ret;

bool __fastcall model_slot7(Inner* self, int edx, CTCCreatureNavigation* owner, C3DVector* dest)
{
  if (self == g_inner && owner == g_owner && dest == g_dest) return g_ret;
  return false;
}

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* dest)
{
  Inner* inner = self->f4;
  return inner->vtbl->slot7(inner, 0, self, dest);
}

int main(){
  InnerVtbl vt; vt.slot7 = model_slot7;
  Inner inner; inner.vtbl = &vt;
  CTCCreatureNavigation owner; owner.f0=0; owner.f4=&inner;
  C3DVector d; d.x=1;d.y=2;d.z=3;
  g_owner=&owner; g_inner=&inner; g_dest=&d;
  g_ret=true;
  bool r1 = IsNewDestinationGoingToResetNavigation(&owner,0,&d);
  g_ret=false;
  bool r2 = IsNewDestinationGoingToResetNavigation(&owner,0,&d);
  if (r1==true && r2==false) printf("OK_00c46a10\n");
  else { printf("FAIL %d %d\n", (int)r1,(int)r2); return 1; }
  return 0;
}