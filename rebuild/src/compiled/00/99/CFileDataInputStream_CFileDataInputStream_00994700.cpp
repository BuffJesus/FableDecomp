// CFileDataInputStream::CFileDataInputStream @ 0x00994700.
//
// LoadGameStateInternal wraps its CDiskFileWin32 with this buffered stream
// immediately after validating the save header.  Retail constructs CBase,
// initializes the CDataInputStream-compatible prefix, retains the CAFile
// pointer, and conditionally allocates/configures the source window.

extern "C" void* __fastcall
CFileDataInputStream_ConstructBase_00994700(void* stream, void*);
extern "C" void* CFileDataInputStream_VTable_00994700[];
extern "C" void* __cdecl
CFileDataInputStream_AllocateBuffer_00994700(unsigned long size);
extern "C" void __fastcall
CFileDataInputStream_SetPosition_00994700(
    void* stream,
    void*,
    unsigned long position);

extern "C" __declspec(naked) void* __fastcall
CFileDataInputStream_CFileDataInputStream_00994700(
    void*,
    void*,
    void*,
    unsigned long)
{
    __asm
    {
        push ebx
        push ebp
        push esi
        push edi
        mov esi, ecx
        call CFileDataInputStream_ConstructBase_00994700
        mov ebp, dword ptr [esp + 18h]
        mov edi, dword ptr [esp + 14h]
        xor ebx, ebx
        cmp ebp, ebx
        mov dword ptr [esi + 4], ebx
        mov dword ptr [esi + 8], ebx
        mov dword ptr [esi + 0Ch], ebx
        mov dword ptr [esi + 10h], ebx
        mov dword ptr [esi + 14h], ebx
        mov dword ptr [esi], offset CFileDataInputStream_VTable_00994700
        mov dword ptr [esi + 18h], edi
        jbe construction_done

        push ebp
        call CFileDataInputStream_AllocateBuffer_00994700
        mov dword ptr [esi + 1Ch], eax
        mov dword ptr [esi + 20h], ebp
        mov eax, dword ptr [edi]
        add esp, 4
        mov ecx, edi
        call dword ptr [eax + 24h]
        mov ecx, dword ptr [esi + 1Ch]
        mov dword ptr [esi + 0Ch], ecx
        mov dword ptr [esi + 4], ebx
        mov dword ptr [esi + 10h], ebx
        mov dword ptr [esi + 8], eax
        mov dword ptr [esi + 14h], ebx
        mov edx, dword ptr [edi]
        mov ecx, edi
        call dword ptr [edx + 1Ch]
        push eax
        mov ecx, esi
        call CFileDataInputStream_SetPosition_00994700

    construction_done:
        pop edi
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        ret 8
    }
}
