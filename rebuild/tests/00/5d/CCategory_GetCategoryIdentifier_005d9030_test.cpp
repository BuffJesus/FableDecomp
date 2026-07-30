#include <stdio.h>

__declspec(naked) int __fastcall GetCategoryIdentifier(void* self)
{
    __asm {
        mov     ecx, dword ptr [ecx+0Ch]
        test    ecx, ecx
        je      skip
        inc     dword ptr [ecx+4]
    skip:
        mov     eax, dword ptr [ecx+4]
        push    esi
        mov     esi, dword ptr [ecx+50h]
        dec     eax
        mov     dword ptr [ecx+4], eax
        jne     done
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax+4]
    done:
        mov     eax, esi
        pop     esi
        ret
    }
}

/* held object layout: [0]=vtbl, [4]=refc, [0x50]=identifier */
static int g_released = 0;

/* virtual slot 1 (offset +4 in vtbl) = Release */
void __stdcall Release_impl() { g_released = 1; }

struct HeldObj {
    void* vtbl;
    int   refc;
    char  gap[0x50 - 8];
    int   identifier;
};

struct Category {
    int a; int b; int c;
    HeldObj* held;
};

/* invoke helper: this=ecx */
static int call_get(void* self)
{
    int r;
    __asm {
        mov  ecx, self
        call GetCategoryIdentifier
        mov  r, eax
    }
    return r;
}

int main()
{
    /* vtable: slot0 unused, slot1 = Release */
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&Release_impl;

    HeldObj obj;
    obj.vtbl = vt;
    obj.refc = 5;            /* non-zero after inc/dec -> Release NOT called */
    obj.identifier = 0x1234ABCD;

    Category cat;
    cat.held = &obj;

    int id = call_get(&cat);

    if (id != 0x1234ABCD) { printf("FAIL id=%08X\n", id); return 1; }
    if (obj.refc != 5)    { printf("FAIL refc=%d\n", obj.refc); return 1; }
    if (g_released != 0)  { printf("FAIL released early\n"); return 1; }

    /* second scenario: refc becomes 0 -> Release IS called */
    HeldObj obj2;
    obj2.vtbl = vt;
    obj2.refc = 0;           /* inc->1, dec->0 -> Release */
    obj2.identifier = 0x55667788;
    Category cat2;
    cat2.held = &obj2;
    g_released = 0;
    int id2 = call_get(&cat2);
    if (id2 != 0x55667788) { printf("FAIL id2=%08X\n", id2); return 1; }
    if (g_released != 1)   { printf("FAIL release-on-zero not called\n"); return 1; }

    printf("OK_0x005d9030 id=%08X id2=%08X\n", id, id2);
    return 0;
}