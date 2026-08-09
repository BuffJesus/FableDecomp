#include <cstdio>

struct C3DVector { float x, y, z; };

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(void* self, int edx, C3DVector* dest) {
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

struct Vtbl { void* s[7]; bool (__stdcall *slot7)(void* outer, C3DVector* dest); };
struct Helper { Vtbl* vtbl; };
struct Nav { void* f0; Helper* helper; };

static void* g_expectedOuter;
static C3DVector* g_expectedDest;
static bool g_ret;
static int g_calls;

bool __stdcall my_slot7(void* outer, C3DVector* dest) {
    g_calls++;
    if (outer != g_expectedOuter) { printf("FAIL outer\n"); }
    if (dest != g_expectedDest) { printf("FAIL dest\n"); }
    return g_ret;
}

int main() {
    Vtbl vt; for (int i=0;i<7;i++) vt.s[i]=0; vt.slot7 = my_slot7;
    Helper h; h.vtbl = &vt;
    Nav nav; nav.f0 = 0; nav.helper = &h;
    C3DVector d; d.x = 1; d.y = 2; d.z = 3;
    g_expectedOuter = &nav; g_expectedDest = &d;

    g_ret = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &d);
    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &d);

    if (r1 == true && r2 == false && g_calls == 2) {
        printf("OK_00c0f0b0\n");
        return 0;
    }
    printf("FAILED\n");
    return 1;
}