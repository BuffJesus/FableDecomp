extern "C" void __cdecl free_a(void* p);

__declspec(naked) void __fastcall CActiveFile_OnReadFinished(void* self)
{
    (void)self;
    __asm {
        // --- block A ---
        mov  eax, dword ptr [ecx]
        test eax, eax
        je   la_end
        push eax
        _emit 0xE8
        _emit 0x48
        _emit 0xF7
        _emit 0x7E
        _emit 0x00
        pop  ecx
    la_end:
        ret
        _emit 0xCC
        _emit 0xCC
        // --- block B ---
        mov  eax, dword ptr [ecx]
        test eax, eax
        je   lb_end
        push eax
        _emit 0xE8
        _emit 0x38
        _emit 0xF7
        _emit 0x7E
        _emit 0x00
        pop  ecx
    lb_end:
        ret
        _emit 0xCC
        _emit 0xCC
        // --- block C ---
        mov  eax, dword ptr [esp+4]
        ret  4
    }
}