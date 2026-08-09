
#include <cstdio>
struct Inner;
struct InnerVtbl {
    void* s0; void* s1; void* s2; void* s3; void* s4; void* s5; void* s6;
    bool (__stdcall* slot7)(void* self, void* dest);
};
struct Inner { InnerVtbl* vtbl; };
struct CTCCreatureNavigation { void* pad0; Inner* m_field4; };
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, void* dest, float f);
static void* g_seen_self; static void* g_seen_dest;
static bool __stdcall myslot7(void* self, void* dest){ g_seen_self=self; g_seen_dest=dest; return true; }
int main(){
  InnerVtbl vt; vt.s0=vt.s1=vt.s2=vt.s3=vt.s4=vt.s5=vt.s6=0; vt.slot7=myslot7;
  Inner inner; inner.vtbl=&vt;
  CTCCreatureNavigation nav; nav.pad0=0; nav.m_field4=&inner;
  int destobj=0;
  bool r=IsNewDestinationGoingToResetNavigation(&nav,0,&destobj,3.5f);
  if(r && g_seen_self==&nav && g_seen_dest==&destobj) printf("OK_00c2ad40\n");
  else { printf("FAIL self=%p dest=%p r=%d\n",g_seen_self,g_seen_dest,(int)r); return 1; }
  return 0;
}