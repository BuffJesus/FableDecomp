// CArray<std::pair<unsigned_long,...> >::push_back @ 0x00476f4f.
//
// __fastcall, 2 stack args (ret 8).  `this` (ecx) holds a {first,last} range of
// 0xc4-byte-stride element slots (std::pair<unsigned_long, ...> whose sizeof is
// 0xc4 = 196):
//   [ecx+0x00] = first  (begin pointer)
//   [ecx+0x04] = last   (end pointer)
// arg0 (index) arrives in [esp+0x10] after the 3 register saves; arg1 (value
// ptr) in [esp+0x14].
//
// This variant differs from its 0x5c / 0x54 siblings in register allocation:
// the stride constant lives in EDI (loaded via `mov edi,0c4h`, an imm32 move,
// not push/pop) and first lives in EBX.  count = (last-first)/0xc4 (signed
// idiv, divisor mirrored into ESI first).  Branch on arg0 vs count:
//   * index <  count : call InsertAt(first + index*0xc4, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, value)[callee B]
// In the append branch retail reloads last into EAX (not EBX) and divides by
// EDI (stride still resident).  Both callees are relocation-masked; declared
// extern so the call rel32 operands mask out.  Modeled as a naked __fastcall so
// ecx=this, edx unused, reproducing the exact prologue/epilogue and register
// usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_00476f4f_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_00476f4f_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00476f4f(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [ecx + 4]
        push ebx
        mov  ebx, dword ptr [ecx]
        push esi
        sub  eax, ebx
        push edi
        mov  edi, 0C4h
        cdq
        mov  esi, edi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  append

        push dword ptr [ecx + 4]
        imul esi, esi, 0C4h
        add  esi, ebx
        push esi
        call CArray_push_back_00476f4f_InsertAt_A
        jmp  done

    append:
        mov  eax, dword ptr [ecx + 4]
        sub  eax, ebx
        cdq
        idiv edi
        push dword ptr [esp + 14h]
        sub  esi, eax
        push esi
        push dword ptr [ecx + 4]
        call CArray_push_back_00476f4f_AppendFill_B

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}