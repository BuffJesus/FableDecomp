// CHoverable::OnUnhovered @ 0x0055b9a0  (__fastcall, this=ecx, no stack args)
//
// Disassembly:
//   push esi
//   mov  esi, ecx
//   mov  ecx, [esi+0x15c]
//   mov  eax, [esi]
//   push ecx
//   mov  ecx, esi
//   call [eax+0xc0]
//   mov  eax, [esi+0x168]
//   mov  edx, [esi]
//   push eax
//   mov  ecx, esi
//   call [edx+0x20c]
//   pop  esi
//   ret
//
// Semantically: this->vf_0xc0( this->field_15c ); this->vf_0x20c( this->field_168 );
// No relocations (all immediates) -> naked transcription reaches MATCH.

__declspec(naked) void OnUnhovered()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esi+0x15c]
        mov  eax, dword ptr [esi]
        push ecx
        mov  ecx, esi
        call dword ptr [eax+0xc0]
        mov  eax, dword ptr [esi+0x168]
        mov  edx, dword ptr [esi]
        push eax
        mov  ecx, esi
        call dword ptr [edx+0x20c]
        pop  esi
        ret
    }
}