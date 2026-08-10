extern "C" { void __fastcall MemberDtor_af8bc0(void* self); void __fastcall BaseDtor_f36c90(void* self); }
__declspec(naked) void __fastcall DtorIPM_C(void* self)
{
    (void)self;
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], 0x129f650
        call MemberDtor_af8bc0
        mov  ecx, esi
        pop  esi
        jmp  BaseDtor_f36c90
    }
}