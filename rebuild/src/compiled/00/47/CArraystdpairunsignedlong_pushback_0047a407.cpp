// CArray<std::pair<unsigned_long,...> >::push_back @ 0x0047a407.
//
// __fastcall, 2 stack args (ret 8).  `this` (ecx) holds a {first,last} range of
// 0x50-byte-stride element slots (std::pair<unsigned_long, ...> whose sizeof is
// 0x50 = 80):
//   [ecx+0x00] = first  (begin pointer)
//   [ecx+0x04] = last   (end pointer)
// arg0 (index) arrives in [esp+0x10] after the 3 register saves; arg1 (value
// ptr) in [esp+0x14].
//
// The retail body computes count = (last-first)/0x50 (signed idiv) and branches
// on arg0 vs count:
//   * index <  count : call InsertAt(first + index*0x50, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, arg1)[callee B]
// Because the stride 0x50 = 5*16, the slot offset index*0x50 is materialised by
// the compiler as lea eax,[esi+esi*4] ; shl eax,4 (not an imul), matching the
// retail 8d 04 b6 / c1 e0 04 bytes exactly.
// Both callees are relocation-masked; declared extern so the call operands mask
// out.  Modeled as a naked __fastcall so ecx=this, edx unused, matching the
// exact prologue/epilogue and register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_0047a407_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_0047a407_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_0047a407(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
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
        call CArray_push_back_0047a407_InsertAt_A
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
        call CArray_push_back_0047a407_AppendFill_B

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}