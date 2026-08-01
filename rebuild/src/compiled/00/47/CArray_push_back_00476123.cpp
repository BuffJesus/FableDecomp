// CArray<std::pair<unsigned_long,...> >::push_back @ 0x00476123.
//
// __thiscall, 2 stack args (ret 8).  `this` (ecx) holds a {first,last} range of
// 0x50-byte-stride element slots (std::pair<unsigned_long, ...> whose sizeof is
// 0x50 = 80):
//   [ecx+0x00] = first  (begin pointer)
//   [ecx+0x04] = last   (end pointer)
// arg0 (index) arrives in [esp+0x10] after the 3 register saves; arg1 (value
// ptr) in [esp+0x14].
//
// Because the stride 0x50 = 5 * 16 factors cleanly, the retail insert path
// computes index*0x50 with `lea eax,[esi+esi*4]` (esi*5) then `shl eax,4`
// (*16) into EAX (not an imul), matching the exact retail bytes.  The count is
// still (last-first)/0x50 via a signed idiv.  Branch on arg0 vs count:
//   * index <  count : call InsertAt(first + index*0x50, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, value)[callee B]
// Both callees are relocation-masked; declared extern so the call operands mask
// out.  Modeled as a naked __fastcall so ecx=this, edx unused, reproducing the
// exact prologue/epilogue and register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_00476123_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_00476123_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00476123(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
{
    __asm
    {
        push ebx
        mov  ebx, dword ptr [ecx + 4]
        push esi
        push edi
        mov  edi, dword ptr [ecx]
        mov  eax, ebx
        sub  eax, edi
        push 50h
        cdq
        pop  esi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  append

        lea  eax, [esi + esi*4]
        shl  eax, 4
        push ebx
        add  eax, edi
        push eax
        call CArray_push_back_00476123_InsertAt_A
        jmp  done

    append:
        mov  ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov  eax, ebx
        sub  eax, edi
        push 50h
        pop  edi
        cdq
        idiv edi
        sub  esi, eax
        push esi
        push ebx
        call CArray_push_back_00476123_AppendFill_B

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}
