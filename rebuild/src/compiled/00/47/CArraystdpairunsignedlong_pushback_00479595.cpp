// CArray<std::pair<unsigned_long,...> >::push_back @ 0x00479595.
//
// __fastcall, 2 stack args (ret 8).  Element stride is 0x50 (80) bytes.  Because
// 80 = 5 * 16, MSVC computes index*80 with an lea+shl pair instead of idiv/imul:
//   count = (last - first) / 0x50                     (signed idiv by 0x50)
//   slot  = first + index*80 = first + (index*5)<<4   (lea [esi+esi*4]; shl 4)
//   [ecx+0x00] = first (begin pointer)
//   [ecx+0x04] = last  (end pointer)
// arg0 (index) at [esp+0x10] after the 3 register pushes; arg1 (value) at [esp+0x14].
//
// Branch on index (unsigned cmp / jae) vs count:
//   * index <  count : call InsertAt(slot=first+index*80, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, value) [callee B]
// Both callees are relocation-masked; declared extern so the call operands mask
// out.  Modeled as a naked __fastcall so ecx=this, edx unused, reproducing the
// exact prologue/epilogue and register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_00479595_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_00479595_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00479595(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
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
        push 50h
        cdq
        pop esi
        idiv esi
        mov esi, dword ptr [esp + 10h]
        cmp esi, eax
        jae append

        lea eax, [esi + esi*4]
        shl eax, 4
        push ebx
        add eax, edi
        push eax
        call CArray_push_back_00479595_InsertAt_A
        jmp done

    append:
        mov ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov eax, ebx
        sub eax, edi
        push 50h
        pop edi
        cdq
        idiv edi
        sub esi, eax
        push esi
        push ebx
        call CArray_push_back_00479595_AppendFill_B

    done:
        pop edi
        pop esi
        pop ebx
        ret 8
    }
}