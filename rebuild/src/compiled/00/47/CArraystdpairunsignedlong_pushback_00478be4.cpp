// CArray<std::pair<unsigned_long,...> >::push_back @ 0x00478be4.
//
// __fastcall, 2 stack args (ret 8).  `this` (ecx) holds a {first,last} range of
// 0x48-byte-stride element slots (std::pair<unsigned_long, ...> whose sizeof is
// 0x48 = 72):
//   [ecx+0x00] = first  (begin pointer)
//   [ecx+0x04] = last   (end pointer)
// arg0 (index) arrives in [esp+0x10] after the 3 register saves; arg1 (value
// ptr) in [esp+0x14].
//
// The retail body computes count = (last-first)/0x48 (signed idiv) and branches
// on arg0 vs count:
//   * index <  count : call InsertAt(first + index*0x48, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, arg1)[callee B]
// 0x48 == 72 == 8*9, so the slot address is built with two LEAs
// (lea eax,[esi+esi*8] -> index*9 ; lea eax,[edi+eax*8] -> first + index*72)
// rather than an imul, matching the retail bytes exactly.
//
// Both callees are relocation-masked; declared extern so the call operands mask
// out.  Modeled as a naked __fastcall so ecx=this, edx unused, matching the
// exact prologue/epilogue and register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_00478be4_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_00478be4_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00478be4(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
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
        push 48h
        cdq
        pop  esi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  append

        lea  eax, [esi + esi*8]
        push ebx
        lea  eax, [edi + eax*8]
        push eax
        call CArray_push_back_00478be4_InsertAt_A
        jmp  done

    append:
        mov  ebx, dword ptr [ecx + 4]
        push dword ptr [esp + 14h]
        mov  eax, ebx
        sub  eax, edi
        push 48h
        pop  edi
        cdq
        idiv edi
        sub  esi, eax
        push esi
        push ebx
        call CArray_push_back_00478be4_AppendFill_B

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}