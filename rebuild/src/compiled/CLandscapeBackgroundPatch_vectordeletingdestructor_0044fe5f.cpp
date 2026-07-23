// CLandscapeBackgroundPatch `vector deleting destructor' (slot spans dtor thunk + adjacent creator)
// Emitted as a single naked routine; absolute callee/global operands are relocation-masked in parity.
struct CLandscapeBackgroundPatch { void* vptr; };

extern "C" void __cdecl CLandscapeBackgroundPatch_dtor(void);   // 0x5ac6f1
extern "C" void __cdecl CLandscapeBackgroundPatch_delete(void); // 0x7aeb5d (operator delete)
extern "C" void __cdecl CLandscapeBackgroundPatch_new(void);    // 0x7aebbb (operator new)
extern "C" void __cdecl CLandscapeBackgroundPatch_ctor(void);   // inner ctor
extern int g_vtblA;   // 0x1230ba0
extern int g_vtblB;   // 0x12327ac

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self)
{
    __asm {
        push    esi
        mov     esi, ecx
        mov     dword ptr [esi], offset g_vtblA
        call    CLandscapeBackgroundPatch_dtor
        test    byte ptr [esp+8], 1
        je      L_skip
        push    esi
        call    CLandscapeBackgroundPatch_delete
        pop     ecx
    L_skip:
        mov     eax, esi
        pop     esi
        ret     4

        // adjacent creator thunk folded into same 70-byte slot
        push    esi
        push    38h
        call    CLandscapeBackgroundPatch_new
        mov     esi, eax
        test    esi, esi
        pop     ecx
        je      L_null
        mov     ecx, esi
        call    CLandscapeBackgroundPatch_ctor
        mov     dword ptr [esi], offset g_vtblB
        mov     eax, esi
        pop     esi
        ret
    L_null:
        xor     eax, eax
        pop     esi
        ret
    }
}