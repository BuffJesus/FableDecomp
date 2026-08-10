#include <stdio.h>
struct C3DVector { float x,y,z; };
struct Nav;
struct NavInner;

typedef unsigned char (__fastcall *SlotFn)(NavInner*, int, Nav*, C3DVector*);

struct NavInner { SlotFn* vtbl; };
struct Nav { void* f0; NavInner* f4; };

static Nav* g_lastThis;
static C3DVector* g_lastDest;
static NavInner* g_lastInner;
static unsigned char g_ret;

static unsigned char __fastcall slot7(NavInner* inner, int /*edx*/, Nav* self, C3DVector* dest)
{
    g_lastInner = inner;
    g_lastDest = dest;
    g_lastThis = self;
    return g_ret;
}

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(Nav* self, int /*edx*/, C3DVector* dest)
{
    (void)self; (void)dest;
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

int main()
{
    static SlotFn vtbl[16];
    for (int i=0;i<16;i++) vtbl[i]=0;
    vtbl[7]=slot7;
    NavInner inner; inner.vtbl=vtbl;
    Nav nav; nav.f0=0; nav.f4=&inner;
    C3DVector dest; dest.x=1;dest.y=2;dest.z=3;

    g_ret=1;
    bool r = IsNewDestinationGoingToResetNavigation(&nav, 0, &dest);
    if (!r) { printf("FAIL ret true\n"); return 1; }
    if (g_lastInner != &inner) { printf("FAIL inner\n"); return 1; }
    if (g_lastDest != &dest) { printf("FAIL dest\n"); return 1; }
    if (g_lastThis != &nav) { printf("FAIL this\n"); return 1; }

    g_ret=0;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &dest);
    if (r2) { printf("FAIL ret false\n"); return 1; }

    printf("OK_00c2bff0\n");
    return 0;
}