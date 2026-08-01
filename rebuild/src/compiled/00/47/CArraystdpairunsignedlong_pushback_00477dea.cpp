// CArray<std::pair<unsigned_long, ...> >::push_back @ 0x00477dea.
//
// __fastcall, 2 stack args (ret 8).  `this` (ecx) holds a {first,last} range of
// 0x88-byte-stride element slots:
//   [ecx+0x00] = first  (begin pointer)
//   [ecx+0x04] = last   (end pointer)
// arg0 (index) arrives at [esp+0x10] after the 3 register saves; arg1 (value
// ptr) at [esp+0x14].
//
// count = (last - first) / 0x88 (signed idiv).  Branch on arg0 vs count:
//   * index <  count : call InsertAt(first + index*0x88, last)   [callee A]
//   * index >= count : call AppendFill(last, index - count, arg1)[callee B]
//
// This variant differs from the 0x54/0x5c push_back siblings in register
// allocation: the stride 0x88 is materialized with `mov edi, 0x88` (imm32)
// rather than push/pop; ebx holds `first`; esi is a scratch copy of the stride
// for the first idiv; and the append branch reuses edi (still 0x88) as the
// divisor without reloading `first`.  Both callees are relocation-masked;
// declared extern so the call operands mask out.  Modeled as a naked __fastcall
// so ecx=this and edx is unused, reproducing the exact prologue/epilogue and
// register usage of the retail bytes.

extern "C" void __fastcall CArray_push_back_00477dea_InsertAt_A(void* slot, void* last);
extern "C" void __fastcall CArray_push_back_00477dea_AppendFill_B(void* last, long fillcount, void* value);

extern "C" __declspec(naked) void __fastcall
CArray_push_back_00477dea(void* /*ecx this*/, void* /*edx*/, long /*index*/, void* /*value*/)
{
    __asm
    {
        mov  eax, dword ptr [ecx + 4]
        push ebx
        mov  ebx, dword ptr [ecx]
        push esi
        sub  eax, ebx
        push edi
        mov  edi, 88h
        cdq
        mov  esi, edi
        idiv esi
        mov  esi, dword ptr [esp + 10h]
        cmp  esi, eax
        jae  append

        push dword ptr [ecx + 4]
        imul esi, esi, 88h
        add  esi, ebx
        push esi
        call CArray_push_back_00477dea_InsertAt_A
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
        call CArray_push_back_00477dea_AppendFill_B

    done:
        pop  edi
        pop  esi
        pop  ebx
        ret  8
    }
}