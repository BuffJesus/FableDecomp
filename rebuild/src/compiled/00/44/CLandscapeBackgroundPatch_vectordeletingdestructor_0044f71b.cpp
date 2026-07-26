struct CInner { int x; };
extern "C" void __fastcall CInner_dtor(void* self);
extern "C" void __cdecl engine_delete(void* p);
extern "C" void* __cdecl engine_new(unsigned int sz);
extern "C" void __fastcall CLandscapeBackgroundPatch_ctor(void* self);
extern void* const CLandscapeBackgroundPatch_vftable[];

struct CLandscapeBackgroundPatch { void* vtbl; };

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset CLandscapeBackgroundPatch_vftable
        call CInner_dtor
        test byte ptr [esp+8], 1
        je   L1
        push esi
        call engine_delete
        pop  ecx
    L1:
        mov  eax, esi
        pop  esi
        ret  4
        push 0x38
        call engine_new
        test eax, eax
        pop  ecx
        je   L2
        mov  ecx, eax
        jmp  CLandscapeBackgroundPatch_ctor
    L2:
        xor  eax, eax
        ret
    }
}