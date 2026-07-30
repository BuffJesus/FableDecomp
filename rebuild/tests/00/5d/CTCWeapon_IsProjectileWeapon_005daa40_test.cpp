#include <stdio.h>

__declspec(naked) bool IsProjectileWeapon()
{
    __asm {
        mov     ecx, dword ptr [ecx+0x0c]
        test    ecx, ecx
        je      L1
        inc     dword ptr [ecx+0x04]
    L1:
        mov     eax, dword ptr [ecx+0x04]
        push    esi
        mov     esi, dword ptr [ecx+0x2c]
        dec     eax
        mov     dword ptr [ecx+0x04], eax
        jne     L2
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax+0x04]
    L2:
        xor     eax, eax
        cmp     esi, 0x03
        sete    al
        pop     esi
        ret
    }
}

struct Weapon { void* vtbl; int refcount; char pad[0x2c-8]; int type; };
struct Outer { char pad[0xc]; Weapon* w; };

static int g_released = 0;
static void __fastcall release_thunk() { g_released++; }
static void* g_vtbl[2];

static bool call_it(Outer* o)
{
    bool r;
    __asm {
        mov ecx, o
        call IsProjectileWeapon
        mov r, al
    }
    return r;
}

int main()
{
    g_vtbl[0] = 0;
    g_vtbl[1] = (void*)&release_thunk;

    Weapon wa; wa.vtbl = g_vtbl; wa.refcount = 5; wa.type = 3;
    Outer oa; oa.w = &wa;
    bool ra = call_it(&oa);
    if (ra != true) { printf("FAIL A ret=%d\n", (int)ra); return 1; }
    if (wa.refcount != 5) { printf("FAIL A refcount=%d\n", wa.refcount); return 1; }
    if (g_released != 0) { printf("FAIL A released=%d\n", g_released); return 1; }

    Weapon wb; wb.vtbl = g_vtbl; wb.refcount = 2; wb.type = 5;
    Outer ob; ob.w = &wb;
    bool rb = call_it(&ob);
    if (rb != false) { printf("FAIL B ret=%d\n", (int)rb); return 1; }

    Weapon wc; wc.vtbl = g_vtbl; wc.refcount = 0; wc.type = 3;
    Outer oc; oc.w = &wc;
    bool rc = call_it(&oc);
    if (rc != true) { printf("FAIL C ret=%d\n", (int)rc); return 1; }
    if (g_released != 1) { printf("FAIL C released=%d\n", g_released); return 1; }

    printf("OK_0x005daa40\n");
    return 0;
}