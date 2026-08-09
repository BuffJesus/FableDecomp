#include <cstdio>
struct C3DVector { float x, y, z; };
struct Inner;
struct InnerVtbl {
  void* s0; void* s1; void* s2; void* s3; void* s4; void* s5;
  bool (__fastcall* check)(Inner*, int, C3DVector*);
};
struct Inner { InnerVtbl* vtbl; };
struct CTCCreatureNavigation { void* pad0; Inner* inner; };

static C3DVector* g_seen;
static Inner* g_self;
static bool g_ret;
static bool __fastcall myCheck(Inner* p, int, C3DVector* v){ g_seen=v; g_self=p; return g_ret; }

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest, float /*dist*/)
{
  Inner* p = self->inner;
  return p->vtbl->check(p, 0, dest);
}

int main(){
  InnerVtbl vt; vt.check = myCheck;
  Inner inner; inner.vtbl = &vt;
  CTCCreatureNavigation nav; nav.pad0=0; nav.inner=&inner;
  C3DVector d = {1.0f,2.0f,3.0f};
  g_ret=true;
  bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &d, 9.0f);
  if(!r1 || g_seen!=&d || g_self!=&inner){ printf("FAIL1\n"); return 1; }
  g_ret=false;
  bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &d, 0.0f);
  if(r2){ printf("FAIL2\n"); return 1; }
  printf("OK_00c0f400\n");
  return 0;
}