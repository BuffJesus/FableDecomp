struct CLandscapeBackgroundPatch { void* vtbl; };
extern "C" void __fastcall eng_dtor(void*);
extern "C" void __cdecl eng_delete(void*);
extern "C" void* __cdecl eng_new(unsigned int);
extern "C" void __fastcall eng_ctor(void*);
extern void* g_vtbl;

__declspec(naked) void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, unsigned char flags)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset g_vtbl
        call eng_dtor
        test byte ptr [esp+8], 1
        je   skip
        push esi
        call eng_delete
        pop  ecx
    skip:
        mov  eax, esi
        pop  esi
        ret  4
        push 60h
        call eng_new
        test eax, eax
        pop  ecx
        je   zero
        mov  ecx, eax
        jmp  eng_ctor
    zero:
        xor  eax, eax
        ret
    }
}