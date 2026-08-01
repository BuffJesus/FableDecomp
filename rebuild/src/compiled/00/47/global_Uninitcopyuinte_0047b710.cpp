// std::_Uninit_copy<_Vector_const_iterator<uint>, uint*, allocator<uint> > @ 0x0047b710.
//
// __fastcall: ecx = first (src _Vector_const_iterator, i.e. src pointer),
//             edx = last  (src end pointer), and one stack arg = dest pointer.
// Returns the advanced dest pointer in eax.  ret 8 pops the single stack arg
// PLUS the (masked) allocator reference the caller pushed -> 2 dwords.
//
// The element being copied is a 12-byte (0x0c) struct made of 3 sub-members at
// offsets +0x00, +0x04, +0x08.  For each element the routine copy-constructs the
// three sub-members individually via a single __fastcall copy helper (masked),
// passing the destination sub-member in ecx and the source sub-member address on
// the stack.  When the dest pointer (edi) is null the per-element copies are
// skipped but the iterators still advance.
//
// Reproduced with naked asm to match the exact prologue/epilogue, the once-only
// `push ebx` above the loop head, the lea addressing, and ret 8.  This mirrors
// the byte-matched sibling _Uninit_copy at 0x0047b670.

extern "C" void __fastcall Uninit_copy_uint_047b710_CopyCtor(void* dst_ecx, void* src_stack);

extern "C" __declspec(naked) void* __fastcall
Uninit_copy_uint_047b710(void* /*ecx first*/, void* /*edx last*/, void* /*dest*/)
{
    __asm
    {
        push ebp
        push esi
        mov ebp, edx
        mov esi, ecx
        cmp esi, ebp
        push edi
        mov edi, dword ptr [esp + 10h]
        je done

        push ebx
    loop_top:
        test edi, edi
        je advance

        push esi
        mov ecx, edi
        call Uninit_copy_uint_047b710_CopyCtor
        lea eax, dword ptr [esi + 4]
        lea ebx, dword ptr [edi + 8]
        push eax
        lea ecx, dword ptr [ebx - 4]
        call Uninit_copy_uint_047b710_CopyCtor
        lea ecx, dword ptr [esi + 8]
        push ecx
        mov ecx, ebx
        call Uninit_copy_uint_047b710_CopyCtor
    advance:
        add esi, 0Ch
        add edi, 0Ch
        cmp esi, ebp
        jne loop_top

        pop ebx
    done:
        mov eax, edi
        pop edi
        pop esi
        pop ebp
        ret 8
    }
}