// std::vector<unsigned char>::insert(position, count, value) @ 0x00485F3A.
//
// LoadGameStateInternal uses this helper to grow its byte vector to the
// combined size of the two compressed save chunks.  The inserted value is
// zero, so newly reserved payload storage is deterministically initialized.

extern "C" void __fastcall
ByteVector_MoveConstructTail_00485f3a(void*, void*, void*);
extern "C" void __fastcall
ByteVector_MoveTailBackward_00485f3a(void*, void*, void*);
extern "C" void __fastcall
ByteVector_FillRange_00485f3a(void*, void*, const unsigned char*);
extern "C" void __fastcall
ByteVector_ReallocateAndInsert_00485f3a(
    void*,
    void*,
    unsigned char*,
    const unsigned char*,
    void*,
    unsigned long,
    int);

extern "C" __declspec(naked) void __fastcall
ByteVector_InsertFill_00485f3a(
    void*,
    void*,
    unsigned char*,
    unsigned long,
    const unsigned char*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov edi, dword ptr [ebp + 0Ch]
        test edi, edi
        mov esi, ecx
        jz insert_done

        mov eax, dword ptr [esi + 4]
        mov ecx, dword ptr [esi + 8]
        sub ecx, eax
        cmp ecx, edi
        jb reallocate

        mov ecx, dword ptr [ebp + 10h]
        sub eax, dword ptr [ebp + 8]
        mov cl, byte ptr [ecx]
        cmp eax, edi
        mov ebx, dword ptr [esi + 4]
        mov byte ptr [ebp + 13h], cl
        mov dword ptr [ebp + 0Ch], eax
        mov edx, ebx
        jbe insertion_reaches_end

        mov ecx, ebx
        sub ecx, edi
        push ebx
        mov dword ptr [ebp + 0Ch], ecx
        call ByteVector_MoveConstructTail_00485f3a
        add dword ptr [esi + 4], edi
        mov esi, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp + 0Ch]
        push ebx
        mov ecx, esi
        call ByteVector_MoveTailBackward_00485f3a
        lea edx, dword ptr [esi + edi]
        mov ecx, esi

    fill_inserted_range:
        lea eax, dword ptr [ebp + 13h]
        push eax
        call ByteVector_FillRange_00485f3a
        jmp insert_done

    insertion_reaches_end:
        lea ecx, dword ptr [ebp + 13h]
        sub edx, eax
        push ecx
        add edx, edi
        mov ecx, ebx
        call ByteVector_FillRange_00485f3a
        sub edi, dword ptr [ebp + 0Ch]
        mov ecx, dword ptr [ebp + 8]
        add dword ptr [esi + 4], edi
        push dword ptr [esi + 4]
        mov edx, ebx
        call ByteVector_MoveConstructTail_00485f3a
        mov eax, dword ptr [ebp + 0Ch]
        add dword ptr [esi + 4], eax
        mov ecx, dword ptr [ebp + 8]
        mov edx, ebx
        jmp fill_inserted_range

    reallocate:
        push 0
        push edi
        lea eax, dword ptr [ebp + 0Bh]
        push eax
        push dword ptr [ebp + 10h]
        mov ecx, esi
        push dword ptr [ebp + 8]
        call ByteVector_ReallocateAndInsert_00485f3a

    insert_done:
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret 0Ch
    }
}
