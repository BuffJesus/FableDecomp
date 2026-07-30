#include <stdio.h>

// ---- function under test (transcription of retail 0x005da7d0) ----
__declspec(naked) int GetCategoryIdentifier()
{
    __asm {
        mov     ecx, dword ptr [ecx+0x0c]
        test    ecx, ecx
        je      skip_addref
        inc     dword ptr [ecx+0x04]
skip_addref:
        mov     eax, dword ptr [ecx+0x04]
        push    esi
        mov     esi, dword ptr [ecx+0x28]
        dec     eax
        mov     dword ptr [ecx+0x04], eax
        jne     skip_release
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax+0x04]
skip_release:
        mov     eax, esi
        pop     esi
        ret
    }
}

// __fastcall shim: this -> ecx
static int call_getid(void* self)
{
    int r;
    __asm {
        mov     ecx, self
        call    GetCategoryIdentifier
        mov     r, eax
    }
    return r;
}

// Refcounted sub-object:
//   +0x00 vtable   +0x04 refcount   +0x28 identifier
struct RcNode {
    void*   vtbl;
    int     refcnt;
    char    pad[0x28 - 0x08];
    int     ident;
};

struct Owner {
    char    pad[0x0c];
    RcNode* node;   // +0x0c
};

static int g_dtor_called = 0;
static void __stdcall fake_dtor() { g_dtor_called = 1; }
static void* g_vtbl[4] = { 0, (void*)&fake_dtor, 0, 0 };

int main()
{
    // Case A: live object (refcount > 1). Read ident; AddRef+Release nets zero; no destruct.
    RcNode objA;
    objA.vtbl   = g_vtbl;
    objA.refcnt = 5;
    objA.ident  = 0x1234ABCD;
    Owner ownerA;
    ownerA.node = &objA;

    int id = call_getid(&ownerA);
    if (id != 0x1234ABCD)   { printf("FAIL: ident got=0x%08X\n", id); return 1; }
    if (objA.refcnt != 5)   { printf("FAIL: refcount changed=%d\n", objA.refcnt); return 1; }
    if (g_dtor_called != 0) { printf("FAIL: dtor on live object\n"); return 1; }

    // Case B: refcount == 0 on entry -> AddRef(->1) then Release(->0) -> vtable slot 1 fires.
    RcNode objB;
    objB.vtbl   = g_vtbl;
    objB.refcnt = 0;
    objB.ident  = 0x55667788;
    Owner ownerB;
    ownerB.node = &objB;
    g_dtor_called = 0;

    int id2 = call_getid(&ownerB);
    if (id2 != 0x55667788)  { printf("FAIL: caseB ident got=0x%08X\n", id2); return 1; }
    if (g_dtor_called != 1) { printf("FAIL: dtor not called at zero\n"); return 1; }

    printf("OK_0x005da7d0 id=0x%08X refcnt=%d dtor=%d\n", id, objA.refcnt, g_dtor_called);
    return 0;
}