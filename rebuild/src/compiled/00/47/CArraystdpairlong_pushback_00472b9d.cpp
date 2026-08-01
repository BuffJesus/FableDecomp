// CArray<std::pair<long,...>>::push_back-family resize helper @ 0x00472B9D.
//
// Retail is __fastcall(this, long count, const void* value) with ret 8.
// Modeled as __fastcall so the naked body can control register/stack usage
// exactly (the edx slot is unused; the disasm clobbers edx with [ecx] first).
// Element stride is 0x40 (64 bytes) -> size = (end - begin) >> 6.
//
//   this+0x00 = begin
//   this+0x04 = end
//
// If count < current size:  erase the tail
//     EraseTail(begin + count*0x40, end)
// else:                     append (count - size) copies of value
//     AppendFill(end, count - size, value)

extern "C" void __cdecl
push_back_00472b9d_EraseTail(void* first, void* last);
extern "C" void __cdecl
push_back_00472b9d_AppendFill(void* dst, long grow, const void* value);

extern "C" __declspec(naked) void __fastcall
push_back_00472b9d(void* /*this ecx*/, void* /*edx*/, long /*count*/, const void* /*value*/)
{
    __asm
    {
        mov edx, [ecx]
        mov eax, [esp + 4]
        push esi
        mov esi, [ecx + 4]
        push edi
        mov edi, esi
        sub edi, edx
        sar edi, 6
        cmp eax, edi
        jae append

        shl eax, 6
        push esi
        add eax, edx
        push eax
        call push_back_00472b9d_EraseTail
        jmp done

    append:
        mov esi, [ecx + 4]
        push dword ptr [esp + 10h]
        mov edi, esi
        sub edi, edx
        sar edi, 6
        sub eax, edi
        push eax
        push esi
        call push_back_00472b9d_AppendFill

    done:
        pop edi
        pop esi
        ret 8
    }
}