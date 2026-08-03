// CComponent::Die  @ 0x00530720  (__fastcall, this=ecx)
//
// Retail disassembly (21 bytes):
//   56                   push esi
//   8b f1                mov  esi, ecx          ; esi = this
//   8b 06                mov  eax, [esi]        ; eax = vtable
//   6a 02                push 2
//   ff 90 c0 00 00 00    call [eax+0xc0]        ; this->vfn_0xc0(2)  (this still in ecx)
//   8b ce                mov  ecx, esi          ; ecx = this
//   5e                   pop  esi
//   e9 bb fc ff ff       jmp  0x005303f0        ; tail-call CComponent::~/dtor-ish helper
//
// The virtual call at slot +0xC0 takes one stack arg (2) and returns; then the
// function tail-jumps to the fixed engine routine at 0x005303f0 with this in ecx.
// The absolute jmp target is a relocation-masked extern.

extern "C" void Die_TailTarget_0x5303f0(void);

__declspec(naked) void Die(void)
{
    __asm
    {
        push esi
        mov  esi, ecx
        mov  eax, dword ptr [esi]
        push 2
        call dword ptr [eax+0x0c0]
        mov  ecx, esi
        pop  esi
        jmp  Die_TailTarget_0x5303f0
    }
}