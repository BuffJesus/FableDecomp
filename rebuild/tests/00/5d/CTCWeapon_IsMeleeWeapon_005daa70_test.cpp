#include <cstdio>

// --- function under test: naked transcription of CTCWeapon::IsMeleeWeapon ---
#if defined(_MSC_VER) && defined(_M_IX86)
__declspec(naked) static bool __fastcall IsMeleeWeapon(void* /*this in ecx*/)
{
    __asm {
        mov  ecx, [ecx+0x0c]
        test ecx, ecx
        je   skip_addref
        inc  dword ptr [ecx+0x04]
    skip_addref:
        mov  eax, [ecx+0x04]
        push esi
        mov  esi, [ecx+0x28]
        dec  eax
        mov  [ecx+0x04], eax
        jne  after_release
        mov  eax, [ecx]
        call dword ptr [eax+0x04]
    after_release:
        cmp  esi, 0x06
        je   is_melee
        cmp  esi, 0x05
        je   is_melee
        xor  al, al
        pop  esi
        ret
    is_melee:
        mov  al, 0x01
        pop  esi
        ret
    }
}
#endif

// A refcounted sub-object: vtable at +0x00, refcount at +0x04, class at +0x28.
struct SubObj {
    void** vtable;      // +0x00
    int    refcount;    // +0x04
    char   pad[0x28-0x08];
    int    weaponClass; // +0x28
};

// Weapon "this": the sub-object pointer lives at +0x0C.
struct Weapon {
    char   pad0[0x0c];
    SubObj* sub;        // +0x0c
};

static int g_released = 0;
// virtual slot 1 ([vtable+4]) = release. __fastcall/naked callee, receives this in ecx.
static void __fastcall ReleaseFn(void*) { g_released++; }

static bool call_it(void* self)
{
    bool r;
    __asm {
        mov ecx, self
        call IsMeleeWeapon
        mov r, al
    }
    return r;
}

int main()
{
    // vtable: slot0 unused, slot1 = release.
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&ReleaseFn;

    SubObj so;
    so.vtable = vt;

    Weapon w;
    w.sub = &so;

    // Case 1: class 6 (melee), refcount high so no release fires.
    so.refcount = 100; so.weaponClass = 6; g_released = 0;
    bool r6 = call_it(&w);
    // AddRef then Release: net refcount unchanged, no destroy.
    if (!(r6 == true && so.refcount == 100 && g_released == 0)) { printf("FAIL case6\n"); return 1; }

    // Case 2: class 5 (melee).
    so.refcount = 100; so.weaponClass = 5; g_released = 0;
    if (call_it(&w) != true) { printf("FAIL case5\n"); return 1; }

    // Case 3: class 3 (non-melee) returns false.
    so.refcount = 100; so.weaponClass = 3; g_released = 0;
    if (call_it(&w) != false) { printf("FAIL case3\n"); return 1; }

    // Case 4: refcount==0 after the bracket triggers the virtual release.
    // Start at 0: inc to 1, dec to 0, release fires exactly once.
    so.refcount = 0; so.weaponClass = 6; g_released = 0;
    bool r4 = call_it(&w);
    if (!(r4 == true && g_released == 1 && so.refcount == 0)) { printf("FAIL case_release g=%d rc=%d\n", g_released, so.refcount); return 1; }

    printf("OK_0x005daa70\n");
    return 0;
}