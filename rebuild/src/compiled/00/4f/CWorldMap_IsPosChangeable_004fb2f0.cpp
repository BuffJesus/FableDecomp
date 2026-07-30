// CWorldMap::IsPosChangeable  0x004fb2f0
// __fastcall(this=ecx, arg pos); ret 4
// Loads vtable [this], calls vf44(pos) then vf60(vf44result)
__declspec(naked) int IsPosChangeable(int pos)
{
    (void)pos;
    __asm {
        mov     eax, dword ptr [esp+4]
        push    esi
        push    edi
        mov     esi, ecx
        mov     edi, dword ptr [esi]
        push    eax
        call    dword ptr [edi+0x44]
        push    eax
        mov     ecx, esi
        call    dword ptr [edi+0x60]
        pop     edi
        pop     esi
        ret     4
    }
}