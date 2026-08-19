// std::_Uninit_copy<_Vector_const_iterator<uint>, uint*, allocator<uint> > @ 0x0047b5f0.
//
// __fastcall: ecx = First (src pointer), edx = Last (src end pointer), and one
// stack arg = Dest pointer (plus the masked allocator ref the caller pushed).
// Returns the advanced Dest pointer in eax.  ret 8 pops the two dwords.
//
// The element is a 12-byte (0x0c) struct { uint a; Member b; char c; } at offsets
// +0x00 / +0x04 / +0x08.  Unlike the sibling _Uninit_copy at 0x0047b670 (which
// copy-constructs all three sub-members via the helper), THIS variant raw-copies
// the +0 dword and the +8 byte, and only calls the out-of-line copy helper
// (masked, target 0x523640) for the +4 sub-member, passing &src.b on the stack
// and &dst.b in ecx.  When Dest (edi) is null the per-element copy is skipped
// but the iterators still advance.
//
// Register roles: ebx = Last, esi = First, edi = Dest.  edi (Dest) is loaded
// unconditionally BEFORE the empty-range `je`, so the empty path falls into the
// single `mov eax,edi` epilogue.  Reproduced with naked asm to pin the exact
// prologue/epilogue, the alignment nop at the loop head, the lea addressing and
// ret 8; a C spelling makes cl split the empty case into a second epilogue that
// reloads Dest from [esp+0xc] (69 vs 60 bytes).

extern "C" void __fastcall Uninit_copy_uint_047b5f0_CopyCtor(void* dst_ecx, void* src_stack);

extern "C" __declspec(naked) void* __fastcall
Uninit_copy_uint_047b5f0(void* /*ecx First*/, void* /*edx Last*/, void* /*Dest*/)
{
    __asm
    {
        push ebx
        push esi
        mov ebx, edx
        mov esi, ecx
        cmp esi, ebx
        push edi
        mov edi, dword ptr [esp + 10h]
        je done
        nop
    loop_top:
        test edi, edi
        je advance

        mov eax, dword ptr [esi]
        lea ecx, dword ptr [esi + 4]
        push ecx
        lea ecx, dword ptr [edi + 4]
        mov dword ptr [edi], eax
        call Uninit_copy_uint_047b5f0_CopyCtor
        mov dl, byte ptr [esi + 8]
        mov byte ptr [edi + 8], dl
    advance:
        add esi, 0Ch
        add edi, 0Ch
        cmp esi, ebx
        jne loop_top
    done:
        mov eax, edi
        pop edi
        pop esi
        pop ebx
        ret 8
    }
}