// CDataInputStream::ReadZlibCompressed @ 0x00996230.
//
// Each binary save block carries a four-byte compressed-size prefix.  Retail
// stages that many bytes in a temporary vector, constructs a zlib compressor,
// expands into the caller's fixed destination, reports a size mismatch through
// the optional callback, and releases the staging vector.

extern "C" void __fastcall
CDataInputStream_ReadOverflow_00996230(
    void*,
    void*,
    unsigned char*,
    long);
extern "C" void __fastcall
CDataInputStream_InitCompressedVector_00996230(
    void*,
    void*,
    unsigned long);
extern "C" void* __cdecl
CDataInputStream_ConstructCompressorBase_00996230();
extern "C" void __fastcall
CDataInputStream_ConstructCompressor_00996230(
    void*,
    void*,
    void*);
extern "C" void* CDataInputStream_CompressorVTable_00996230[];
extern "C" void __fastcall
CDataInputStream_InitCompressor_00996230(void*, void*);
extern "C" unsigned long __fastcall
CDataInputStream_InternalDecompress_00996230(
    void*,
    void*,
    const void*,
    unsigned long,
    void*,
    unsigned long);
extern "C" void* CDataInputStream_ZlibSizeMismatchCallback_00996230;
extern "C" void __fastcall
CDataInputStream_DestroyCompressor_00996230(void*, void*);
extern "C" void __cdecl
CDataInputStream_FreeCompressedBuffer_00996230(void*);

extern "C" __declspec(naked) void __fastcall
CDataInputStream_ReadZlibCompressed_00996230(
    void*,
    void*,
    void*,
    long)
{
    __asm
    {
        sub esp, 68h
        push ebx
        push esi
        mov esi, dword ptr [esp + 78h]
        test esi, esi
        mov ebx, ecx
        jle release_done

        mov eax, dword ptr [ebx + 4]
        lea ecx, dword ptr [eax + 4]
        xor eax, eax
        push ebp
        jl header_read_skipped
        jg header_position_high_valid
        test ecx, ecx
        jb header_read_skipped

    header_position_high_valid:
        test eax, eax
        jg header_read_skipped
        jl header_position_valid
        cmp ecx, 7FFFFFFFh
        ja header_read_skipped

    header_position_valid:
        mov edx, dword ptr [ebx + 14h]
        cmp edx, 4
        jl read_header_overflow
        mov eax, dword ptr [ebx + 0Ch]
        mov ebp, dword ptr [eax]
        add eax, 4
        add edx, -4
        mov dword ptr [ebx + 0Ch], eax
        mov dword ptr [ebx + 14h], edx
        mov dword ptr [ebx + 4], ecx
        jmp header_ready

    read_header_overflow:
        push 4
        lea eax, dword ptr [esp + 10h]
        push eax
        mov ecx, ebx
        call CDataInputStream_ReadOverflow_00996230

    header_read_skipped:
        mov ebp, dword ptr [esp + 0Ch]

    header_ready:
        push edi
        push ebp
        lea ecx, dword ptr [esp + 18h]
        call CDataInputStream_InitCompressedVector_00996230
        mov eax, dword ptr [esp + 14h]
        mov ecx, dword ptr [esp + 18h]
        sub ecx, eax
        mov edi, eax
        jne compressed_destination_ready
        lea edi, dword ptr [esp + 14h]

    compressed_destination_ready:
        test ebp, ebp
        jle compressed_bytes_ready
        mov eax, dword ptr [ebx + 4]
        add eax, ebp
        xor ecx, ecx
        jl compressed_bytes_ready
        jg compressed_position_high_valid
        test eax, eax
        jb compressed_bytes_ready

    compressed_position_high_valid:
        test ecx, ecx
        jg compressed_bytes_ready
        jl compressed_position_valid
        cmp eax, 7FFFFFFFh
        ja compressed_bytes_ready

    compressed_position_valid:
        cmp ebp, dword ptr [ebx + 14h]
        jg read_compressed_overflow
        mov esi, dword ptr [ebx + 0Ch]
        mov ecx, ebp
        mov edx, ecx
        shr ecx, 2
        rep movsd
        mov ecx, edx
        and ecx, 3
        rep movsb
        mov edx, dword ptr [ebx + 0Ch]
        mov ecx, dword ptr [ebx + 14h]
        mov eax, dword ptr [ebx + 4]
        mov esi, dword ptr [esp + 80h]
        add edx, ebp
        sub ecx, ebp
        add eax, ebp
        mov dword ptr [ebx + 0Ch], edx
        mov dword ptr [ebx + 14h], ecx
        mov dword ptr [ebx + 4], eax
        jmp compressed_bytes_ready

    read_compressed_overflow:
        push ebp
        push edi
        mov ecx, ebx
        call CDataInputStream_ReadOverflow_00996230

    compressed_bytes_ready:
        call CDataInputStream_ConstructCompressorBase_00996230
        push eax
        lea ecx, dword ptr [esp + 24h]
        call CDataInputStream_ConstructCompressor_00996230
        lea ecx, dword ptr [esp + 20h]
        mov dword ptr [esp + 20h], offset CDataInputStream_CompressorVTable_00996230
        call CDataInputStream_InitCompressor_00996230
        mov ecx, dword ptr [esp + 18h]
        mov eax, dword ptr [esp + 14h]
        sub ecx, eax
        pop edi
        jne have_compressed_buffer
        lea eax, dword ptr [esp + 10h]

    have_compressed_buffer:
        mov edx, dword ptr [esp + 78h]
        push esi
        push edx
        push ebp
        push eax
        lea ecx, dword ptr [esp + 2Ch]
        call CDataInputStream_InternalDecompress_00996230
        cmp eax, esi
        pop ebp
        je decompression_size_valid
        mov eax, dword ptr [CDataInputStream_ZlibSizeMismatchCallback_00996230]
        test eax, eax
        je decompression_size_valid
        call eax

    decompression_size_valid:
        lea ecx, dword ptr [esp + 18h]
        call CDataInputStream_DestroyCompressor_00996230
        mov eax, dword ptr [esp + 0Ch]
        test eax, eax
        je release_done
        push eax
        call CDataInputStream_FreeCompressedBuffer_00996230
        add esp, 4

    release_done:
        pop esi
        pop ebx
        add esp, 68h
        ret 8
    }
}
