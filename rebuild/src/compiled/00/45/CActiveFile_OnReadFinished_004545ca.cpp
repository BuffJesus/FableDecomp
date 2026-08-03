extern void __cdecl eng_free(void*);

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
    }
}
