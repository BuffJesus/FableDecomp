#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
typedef bool (__stdcall *NavFn)(CTCCreatureNavigation* nav, C3DVector* dest);
struct Dispatch { NavFn slot[8]; };
struct Handler { Dispatch* disp; };
struct CTCCreatureNavigation { void* field_0; Handler* field_4; };
static CTCCreatureNavigation* g_expect_self;
static C3DVector* g_expect_dest;
static bool g_ret;
static bool __stdcall check_impl(CTCCreatureNavigation* nav, C3DVector* dest) {
  if (nav != g_expect_self) { printf("BAD self\n"); return false; }
  if (dest != g_expect_dest) { printf("BAD dest\n"); return false; }
  return g_ret;
}
__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, void* dest) {
  __asm {
    push esi
    mov esi, dword ptr [esp+8]
    mov eax, ecx
    mov ecx, dword ptr [eax+4]
    mov edx, dword ptr [ecx]
    push esi
    push eax
    call dword ptr [edx+0x1c]
    pop esi
    ret 4
  }
}
static bool CallNav(CTCCreatureNavigation* self, C3DVector* dest) {
  return IsNewDestinationGoingToResetNavigation(self, 0, dest);
}
int main() {
  Dispatch disp; for (int i=0;i<8;i++) disp.slot[i]=0; disp.slot[7]=check_impl;
  Handler h; h.disp = &disp;
  CTCCreatureNavigation nav; nav.field_0 = 0; nav.field_4 = &h;
  C3DVector dv; dv.x=1; dv.y=2; dv.z=3;
  g_expect_self = &nav; g_expect_dest = &dv;
  g_ret = true;
  if (CallNav(&nav, &dv) != true) { printf("FAIL1\n"); return 1; }
  g_ret = false;
  if (CallNav(&nav, &dv) != false) { printf("FAIL2\n"); return 1; }
  printf("OK_00c2f700\n");
  return 0;
}