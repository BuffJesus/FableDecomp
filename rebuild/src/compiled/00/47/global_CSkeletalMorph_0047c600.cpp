// std::_Copy_impl<CTCSkeletalMorph::CSkeletalMorph*,CTCSkeletalMorph::CSkeletalMorph*> @ 0x0047c600
//
// __fastcall: ecx = _First (src begin), edx = _Last (src end), stack arg = _Dest.
// Returns the advanced dest pointer in eax. ret 0xc pops the single _Dest stack
// arg PLUS two masked dwords the caller pushed (allocator + slack) => 3 dwords.
//
// Each element is a 0x18-byte (24) CSkeletalMorph made of two {u32; sub-object}
// pairs at offsets +0x00/+0x04 and +0x08/+0x0c. The element count
// = (_Last - _First) / 0x18 is materialised via the signed reciprocal-multiply
// (0x2aaaaaab, sar 2) idiom. For each element the routine raw-copies the two u32
// members and copies the two sub-objects individually via two distinct
// __fastcall copy helpers (masked), passing the destination sub-member in ecx and
// the source sub-member address on the stack.
//
// Reproduced with naked asm to match the exact prologue/epilogue, the once-only
// push ebx/push edi above the loop head, the lea addressing, the two separate
// call targets, and the dual ret 0xc exits.

extern "C" void __fastcall CSkeletalMorph_CopyMemberA(void* dst_ecx, void* src_stack);
extern "C" void __fastcall CSkeletalMorph_CopyMemberB(void* dst_ecx, void* src_stack);

extern "C" __declspec(naked) void* __fastcall
Copy_impl_CSkeletalMorph_0047c600(void* /*ecx _First*/, void* /*edx _Last*/, void* /*_Dest*/)
{
    __asm
    {
        push esi
        mov  esi, ecx
        sub  edx, esi
        mov  eax, 2AAAAAABh
        imul edx
        sar  edx, 2
        mov  eax, edx
        shr  eax, 1Fh
        add  eax, edx
        test eax, eax
        jle  empty

        push ebx
        push edi
        mov  edi, dword ptr [esp + 10h]
        mov  ebx, eax
    loop_top:
        mov  eax, dword ptr [esi]
        lea  ecx, dword ptr [esi + 4]
        push ecx
        lea  ecx, dword ptr [edi + 4]
        mov  dword ptr [edi], eax
        call CSkeletalMorph_CopyMemberA
        mov  edx, dword ptr [esi + 8]
        lea  eax, dword ptr [esi + 0Ch]
        push eax
        lea  ecx, dword ptr [edi + 0Ch]
        mov  dword ptr [edi + 8], edx
        call CSkeletalMorph_CopyMemberB
        add  esi, 18h
        add  edi, 18h
        dec  ebx
        jne  loop_top

        mov  eax, edi
        pop  edi
        pop  ebx
        pop  esi
        ret  0Ch
    empty:
        mov  eax, dword ptr [esp + 8]
        pop  esi
        ret  0Ch
    }
}