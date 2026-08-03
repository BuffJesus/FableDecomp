extern "C" void __cdecl free_a(void* p);

__declspec(naked) void __fastcall CActiveFile_OnReadFinished(void* self)
{
    (void)self;
    __asm {
        mov  eax, dword ptr [ecx]
        test eax, eax
        je   la_end
        push eax
        call free_a
        pop  ecx
    la_end:
        ret
    }
}
