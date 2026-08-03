// std::_Uninit_copy<_Vector_const_iterator<uint>, uint*, allocator<uint> > @ 0x0047c290.
//
// __fastcall: ecx = first (src pointer), edx = last (src end pointer),
//             one stack arg = dest pointer, plus a masked allocator ref.
//             Returns the advanced dest pointer in eax; ret 8 pops 2 dwords.
//
// The element is a 24-byte (0x18) struct.  Per element the routine copies two
// raw uint members (mov [edi],eax and mov [edi+8],edx at +0x00 / +0x08) and
// two sub-members copy-constructed via out-of-line thiscall-style helpers at
// +0x04 / +0x0c (this=&dst.sub in ecx, &src.sub pushed on the stack).  When the
// dest pointer (edi) is null the per-element copies are skipped but the
// iterators still advance.
//
// Reproduced with naked asm to match the exact prologue/epilogue: ebx (not ebp)
// holds Last, the `push edi / mov edi,[esp+10h]` is hoisted above the emptiness
// `je done`, the loop head carries a `nop` alignment pad, and ret 8.  Mirrors
// byte-matched siblings _Uninit_copy at 0x0047b670 / 0x0047b710.

extern "C" void __fastcall Uninit_copy_uint_047c290_CopyA(void* dst_ecx, void* src_stack);
extern "C" void __fastcall Uninit_copy_uint_047c290_CopyB(void* dst_ecx, void* src_stack);

extern "C" __declspec(naked) void* __fastcall
Uninit_copy_uint_047c290(void* /*ecx first*/, void* /*edx last*/, void* /*dest*/)
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
        call Uninit_copy_uint_047c290_CopyA
        mov edx, dword ptr [esi + 8]
        lea eax, dword ptr [esi + 0Ch]
        push eax
        lea ecx, dword ptr [edi + 0Ch]
        mov dword ptr [edi + 8], edx
        call Uninit_copy_uint_047c290_CopyB
    advance:
        add esi, 18h
        add edi, 18h
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