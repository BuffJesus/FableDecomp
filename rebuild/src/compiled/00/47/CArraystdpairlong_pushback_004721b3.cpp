// CArray<std::pair<long,...> >::push_back @ 0x004721b3.
//
// __fastcall, 2 stack args (ret 8).  `this` (ecx) holds a {first,last} range of
// 4-byte-stride element slots:
//   [ecx+0x00] = first  (begin pointer)
//   [ecx+0x04] = last   (end pointer)
// arg0 (index) arrives in [esp+4]; arg1 (value ptr) in the second stack slot.
//
// The retail body computes count = (last-first)>>2 and branches on
// arg0 (unsigned) vs count:
//   * arg0 <  count : call InsertAt(first + arg0*4, last)        [callee A]
//   * arg0 >= count : call AppendFill(last, arg0 - count, arg1)  [callee B]
// Both callees are relocation-masked; declared extern so the call operands mask
// out.  Modeled as a naked __fastcall so ecx=this, edx unused, matching the
// exact prologue/epilogue and register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_004721b3_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_004721b3_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_004721b3(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
{
    __asm
    {
        mov edx, dword ptr [ecx]
        mov eax, dword ptr [esp + 4]
        push esi
        mov esi, dword ptr [ecx + 4]
        push edi
        mov edi, esi
        sub edi, edx
        sar edi, 2
        cmp eax, edi
        jae append

        push esi
        lea eax, [edx + eax*4]
        push eax
        call CArray_push_back_004721b3_InsertAt_A
        jmp done

    append:
        mov esi, dword ptr [ecx + 4]
        push dword ptr [esp + 10h]
        mov edi, esi
        sub edi, edx
        sar edi, 2
        sub eax, edi
        push eax
        push esi
        call CArray_push_back_004721b3_AppendFill_B

    done:
        pop edi
        pop esi
        ret 8
    }
}