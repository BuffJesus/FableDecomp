typedef void (__fastcall *thiscall_fn)(void*);
static void __fastcall member_cleanup(void* self){ (void)self; }
static void __fastcall base_dtor(void* self){ (void)self; }

__declspec(naked) void DtorIPM_E(void* /*self*/)
{
    __asm {
        push esi
        mov esi, ecx
        mov dword ptr [esi], 0x12a20f0
        call member_cleanup
        mov ecx, esi
        pop esi
        jmp base_dtor
    }
}