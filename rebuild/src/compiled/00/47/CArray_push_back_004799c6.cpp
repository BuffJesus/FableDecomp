// CArray<std::pair<unsigned_long,...> >::push_back @ 0x004799c6.
//
// __thiscall, 2 stack args (ret 8).  Element stride is 0x54 (84) bytes, so the
// count is computed by a real signed idiv (not a shift):
//   [ecx+0x00] = first (begin pointer)
//   [ecx+0x04] = last  (end pointer)
//   count = (last - first) / 0x54
// arg0 (index) arrives at [esp+0x10] after the 3 register pushes; arg1 (value
// ptr) at [esp+0x14].
//
// Branch on index (signed cmp / jae -> unsigned) vs count:
//   * index <  count : call InsertAt(first + index*0x54, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, value)[callee B]
// Both callees are relocation-masked; declared extern so the call operands mask
// out.  Modeled as a naked __fastcall so ecx=this, edx unused, reproducing the
// exact prologue/epilogue and register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_004799c6_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_004799c6_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_004799c6(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
{
    __asm
    {
        push ebx
        mov ebx, dword ptr [ecx + 4]
        push esi
        push edi
        mov edi, dword ptr [ecx]
        mov eax, ebx
        sub eax, edi
        push 54h
        cdq
        pop esi
        idiv esi
        mov esi, dword ptr [esp + 10h]
        cmp esi, eax
        jae append

        imul esi, esi, 54h
        push ebx
        add esi, edi
        push esi
        call CArray_push_back_004799c6_InsertAt_A
        jmp done

    append:
        mov ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov eax, ebx
        sub eax, edi
        push 54h
        pop edi
        cdq
        idiv edi
        sub esi, eax
        push esi
        push ebx
        call CArray_push_back_004799c6_AppendFill_B

    done:
        pop edi
        pop esi
        pop ebx
        ret 8
    }
}
