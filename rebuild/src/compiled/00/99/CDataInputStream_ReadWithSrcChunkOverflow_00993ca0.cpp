// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
//
// The save loader reaches this helper when a framing value straddles the
// current CFileDataInputStream source window.  Retail first drains any bytes
// left in that window, then either performs one unbuffered read or repeatedly
// acquires and consumes source chunks.

extern "C" __declspec(naked) void __fastcall
CDataInputStream_ReadWithSrcChunkOverflow_00993ca0(
    void*,
    void*,
    unsigned char*,
    long)
{
    __asm
    {
        push ebx
        push ebp
        push esi
        mov ebx, ecx
        mov eax, dword ptr [ebx + 14h]
        xor esi, esi
        cmp eax, esi
        push edi
        jle no_initial_chunk

        mov edi, dword ptr [esp + 14h]
        mov edx, dword ptr [esp + 18h]
        lea ecx, dword ptr [eax + edi]
        mov dword ptr [esp + 14h], ecx
        mov ecx, dword ptr [ebx + 4]
        sub edx, eax
        add ecx, eax
        cmp esi, esi
        jl initial_copy_rejected
        jg initial_high_valid
        cmp ecx, esi
        jb initial_copy_rejected

    initial_high_valid:
        cmp esi, esi
        jg initial_copy_rejected
        jl initial_copy_valid
        cmp ecx, 7FFFFFFFh
        ja initial_copy_rejected

    initial_copy_valid:
        mov esi, dword ptr [ebx + 0Ch]
        mov ecx, eax
        mov ebp, ecx
        shr ecx, 2
        rep movsd
        mov ecx, ebp
        and ecx, 3
        rep movsb
        mov esi, dword ptr [ebx + 14h]
        mov edi, dword ptr [ebx + 0Ch]
        mov ecx, dword ptr [ebx + 4]
        sub esi, eax
        add edi, eax
        add ecx, eax
        mov dword ptr [ebx + 14h], esi
        mov dword ptr [ebx + 0Ch], edi
        mov dword ptr [ebx + 4], ecx
        xor esi, esi

    initial_copy_rejected:
        mov eax, dword ptr [esp + 14h]
        mov edi, edx
        mov dword ptr [esp + 14h], eax
        mov dword ptr [esp + 18h], edi
        jmp choose_read_path

    no_initial_chunk:
        mov edi, dword ptr [esp + 18h]

    choose_read_path:
        mov edx, dword ptr [ebx]
        push edi
        mov ecx, ebx
        call dword ptr [edx + 24h]
        test al, al
        jz read_unbuffered
        cmp edi, esi
        jle final_done

    read_next_chunk:
        mov eax, dword ptr [ebx + 4]
        mov edx, dword ptr [ebx]
        mov dword ptr [ebx + 10h], eax
        lea esi, dword ptr [ebx + 14h]
        push esi
        lea eax, dword ptr [ebx + 0Ch]
        push eax
        mov ecx, ebx
        call dword ptr [edx + 20h]
        mov esi, dword ptr [esi]
        cmp edi, esi
        mov ebp, edi
        jl have_copy_size
        mov ebp, esi

    have_copy_size:
        test ebp, ebp
        jle advance_destination
        mov eax, dword ptr [ebx + 4]
        add eax, ebp
        xor ecx, ecx
        jl advance_destination
        jg copy_high_valid
        test eax, eax
        jb advance_destination

    copy_high_valid:
        test ecx, ecx
        jg advance_destination
        jl copy_bounds_valid
        cmp eax, 7FFFFFFFh
        ja advance_destination

    copy_bounds_valid:
        cmp ebp, esi
        jg recurse_for_copy
        mov esi, dword ptr [ebx + 0Ch]
        mov edi, dword ptr [esp + 14h]
        mov ecx, ebp
        mov eax, ecx
        shr ecx, 2
        rep movsd
        mov ecx, eax
        and ecx, 3
        rep movsb
        mov edx, dword ptr [ebx + 0Ch]
        mov ecx, dword ptr [ebx + 14h]
        mov eax, dword ptr [ebx + 4]
        mov edi, dword ptr [esp + 18h]
        add edx, ebp
        sub ecx, ebp
        add eax, ebp
        mov dword ptr [ebx + 0Ch], edx
        mov dword ptr [ebx + 14h], ecx
        mov dword ptr [ebx + 4], eax
        jmp advance_destination

    recurse_for_copy:
        mov ecx, dword ptr [esp + 14h]
        push ebp
        push ecx
        mov ecx, ebx
        call CDataInputStream_ReadWithSrcChunkOverflow_00993ca0

    advance_destination:
        mov edx, dword ptr [esp + 14h]
        sub edi, ebp
        add edx, ebp
        test edi, edi
        mov dword ptr [esp + 14h], edx
        mov dword ptr [esp + 18h], edi
        jg read_next_chunk

    read_done:
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret 8

    read_unbuffered:
        mov eax, dword ptr [esp + 14h]
        mov edx, dword ptr [ebx]
        mov ebp, dword ptr [ebx + 4]
        push edi
        push eax
        mov ecx, ebx
        call dword ptr [edx + 28h]
        add ebp, edi
        mov dword ptr [ebx + 4], ebp
        mov dword ptr [ebx + 14h], esi
        mov dword ptr [ebx + 0Ch], esi
        mov dword ptr [ebx + 10h], esi

    final_done:
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret 8
    }
}
