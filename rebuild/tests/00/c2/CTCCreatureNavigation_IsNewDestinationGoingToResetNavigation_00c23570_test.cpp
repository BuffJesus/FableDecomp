#include <stdio.h>
struct C3DVector { float x, y, z; };
struct Inner;
typedef bool (__stdcall *Slot7Fn)(void* self, C3DVector* dest);
struct Inner { Slot7Fn* vtbl; };
struct CTCCreatureNavigation { void* pad0; Inner* field4; };

__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* dest)
{
    (void)self; (void)edx; (void)dest;
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

static void* g_expectedSelf;
static C3DVector* g_expectedDest;
static bool g_retval;

bool __stdcall Slot7Impl(void* self, C3DVector* dest)
{
    if (self != g_expectedSelf) { printf("BAD_SELF\n"); return false; }
    if (dest != g_expectedDest) { printf("BAD_DEST\n"); return false; }
    return g_retval;
}

int main()
{
    Slot7Fn vt[8];
    for (int i = 0; i < 8; ++i) vt[i] = 0;
    vt[7] = Slot7Impl;
    Inner inner; inner.vtbl = vt;
    CTCCreatureNavigation nav; nav.pad0 = 0; nav.field4 = &inner;
    C3DVector dest = {1.0f, 2.0f, 3.0f};
    g_expectedSelf = &nav;
    g_expectedDest = &dest;

    g_retval = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &dest);
    g_retval = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &dest);

    if (r1 == true && r2 == false) { printf("OK_00c23570\n"); return 0; }
    printf("FAIL r1=%d r2=%d\n", (int)r1, (int)r2);
    return 1;
}