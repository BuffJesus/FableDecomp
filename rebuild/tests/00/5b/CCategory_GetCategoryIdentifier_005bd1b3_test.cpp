#include <stdio.h>

// ---- function under test (same body as source_cpp) ----
__declspec(naked) int __fastcall GetCategoryIdentifier(void *thisptr)
{
    (void)thisptr;
    __asm {
        mov     ecx, dword ptr [ecx + 0x14]
        test    ecx, ecx
        je      L_skip_addref
        inc     dword ptr [ecx + 0x4]
    L_skip_addref:
        push    esi
        mov     esi, dword ptr [ecx + 0x50]
        dec     dword ptr [ecx + 0x4]
        jne     L_done
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax + 0x4]
    L_done:
        mov     eax, esi
        pop     esi
        ret
    }
}

// Ref-counted target layout: +0x00 vtable, +0x04 refcount, +0x50 identifier.
struct Target {
    void   *vtable;       // +0x00
    int     refcount;     // +0x04
    char    pad[0x50 - 8];// pad up to +0x50
    int     identifier;   // +0x50
};

// Category layout: identifier accessor reads pointer at this+0x14.
struct Category {
    char    pad[0x14];    // +0x00 .. +0x13
    Target *target;       // +0x14
};

static int g_dtor_called = 0;
static void dummy_slot0() {}
static void dtor_slot()  { g_dtor_called = 1; }
// vtable: slot0 at +0x00, dtor at +0x04
static void *g_vtable[2] = { (void*)&dummy_slot0, (void*)&dtor_slot };

// __fastcall shim: pass 'this' in ecx, no stack args.
static int call_get(void *self)
{
    int result;
    __asm {
        mov  ecx, self
        call GetCategoryIdentifier
        mov  result, eax
    }
    return result;
}

int main()
{
    // Case 1: refcount stays > 0 after the scoped ref, no destructor call.
    Target t;
    t.vtable     = (void*)g_vtable;
    t.refcount   = 5;          // starts at 5; inc->6, dec->5 (nonzero)
    t.identifier = 0x1234ABCD;

    Category c;
    c.target = &t;

    g_dtor_called = 0;
    int id = call_get(&c);

    if (id != 0x1234ABCD)        { printf("FAIL id=%08x\n", id); return 1; }
    if (t.refcount != 5)         { printf("FAIL rc=%d\n", t.refcount); return 1; }
    if (g_dtor_called != 0)      { printf("FAIL dtor unexpectedly called\n"); return 1; }

    // Case 2: refcount reaches zero after Release -> destructor slot fires.
    Target t2;
    t2.vtable     = (void*)g_vtable;
    t2.refcount   = 0;         // inc->1, dec->0 -> call vtable+4
    t2.identifier = 0x00BEEF01;

    Category c2;
    c2.target = &t2;

    g_dtor_called = 0;
    int id2 = call_get(&c2);

    if (id2 != 0x00BEEF01)       { printf("FAIL id2=%08x\n", id2); return 1; }
    if (t2.refcount != 0)        { printf("FAIL rc2=%d\n", t2.refcount); return 1; }
    if (g_dtor_called != 1)      { printf("FAIL dtor not called\n"); return 1; }

    printf("OK_0x005bd1b3 id=%08x id2=%08x\n", id, id2);
    return 0;
}