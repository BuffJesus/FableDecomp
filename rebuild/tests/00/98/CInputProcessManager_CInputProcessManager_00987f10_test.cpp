#include <cstdio>
struct CInputProcessManager { void* vtbl; };
static int g_sub=0, g_base=0;
static CInputProcessManager* g_last_sub=0;
static CInputProcessManager* g_last_base=0;
extern "C" void __fastcall sub_A5C600(CInputProcessManager* self){ g_sub++; g_last_sub=self; }
extern "C" void __fastcall base_dtor_99A300(CInputProcessManager* self){ g_base++; g_last_base=self; }
void __fastcall DtorInputProcessManager2(CInputProcessManager* self)
{
  self->vtbl = (void*)0x129a064;
  sub_A5C600(self);
  base_dtor_99A300(self);
}
int main(){
  CInputProcessManager obj; obj.vtbl=0;
  DtorInputProcessManager2(&obj);
  int ok=1;
  if (obj.vtbl != (void*)0x129a064) ok=0;
  if (g_sub != 1) ok=0;
  if (g_base != 1) ok=0;
  if (g_last_sub != &obj) ok=0;
  if (g_last_base != &obj) ok=0;
  if (ok) { printf("OK_00987f10\n"); return 0; }
  printf("FAIL_00987f10\n"); return 1;
}