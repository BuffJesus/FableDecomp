extern void __cdecl eng_free(void*);
extern void* __cdecl eng_op_new(unsigned int);
extern void* __fastcall CActiveFile_new_ctor(void*);

__declspec(naked) void __fastcall CActiveFile_OnReadFinished() {
    __asm {
        mov eax, dword ptr [ecx]
        test eax, eax
        je   L1
        push eax
        call eng_free
        pop  ecx
    L1:
        ret
        push 0x3c
        call eng_op_new
        test eax, eax
        pop  ecx
        je   L2
        mov  ecx, eax
        jmp  CActiveFile_new_ctor
    L2:
        xor  eax, eax
        ret
    }
}