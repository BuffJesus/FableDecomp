// CDiskFileWin32::Read @ 0x0099A960.
//
// LoadGameStateInternal first uses this for the eight-byte save header.  A
// Retail validates the destination first; after that a zero-length request is
// a no-op.  It reports reads at EOF via CAFileException, calls Win32 ReadFile,
// reports API failure, and advances +8 by ReadFile's actual byte count.

extern "C" const char CDiskFileWin32_NullDestination_0099a960[];
extern "C" const char CDiskFileWin32_ReadPastEnd_0099a960[];
extern "C" const char CDiskFileWin32_ReadFailure_0099a960[];
extern "C" unsigned char CDiskFileWin32_ThrowInfo_0099a960[];
extern "C" void* CDiskFileWin32_ReadFileIAT_0099a960;

extern "C" void* __fastcall
CDiskFileWin32_ConstructErrorString_0099a960(
    void* destination,
    void*,
    const char* text,
    int length);
extern "C" void* __fastcall
CDiskFileWin32_CopyErrorString_0099a960(
    void* destination,
    void*,
    const void* source);
extern "C" char* __fastcall
CDiskFileWin32_GetErrorText_0099a960(void* value, void*);
extern "C" void __fastcall
CDiskFileWin32_DestroyErrorString_0099a960(void* value, void*);
extern "C" void __stdcall
CDiskFileWin32_ThrowError_0099a960(void* exceptionValue, void* throwInfo);

extern "C" __declspec(naked) void __fastcall
CDiskFileWin32_Read_0099a960(
    void*,
    void*,
    void*,
    long,
    void*)
{
    __asm
    {
        sub esp, 8
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esp + 10h]
        test ecx, ecx
        jne destination_valid

        push -1
        push offset CDiskFileWin32_NullDestination_0099a960
        lea ecx, dword ptr [esp + 1Ch]
        call CDiskFileWin32_ConstructErrorString_0099a960
        lea eax, dword ptr [esp + 14h]
        push eax
        lea ecx, dword ptr [esp + 14h]
        call CDiskFileWin32_CopyErrorString_0099a960
        lea ecx, dword ptr [esp + 10h]
        call CDiskFileWin32_GetErrorText_0099a960
        lea ecx, dword ptr [esp + 10h]
        call CDiskFileWin32_DestroyErrorString_0099a960
        push offset CDiskFileWin32_ThrowInfo_0099a960
        lea ecx, dword ptr [esp + 8]
        push ecx
        mov dword ptr [esp + 0Ch], 1
        call CDiskFileWin32_ThrowError_0099a960

    destination_valid:
        mov eax, dword ptr [esp + 14h]
        test eax, eax
        je read_done
        mov edx, dword ptr [esi + 8]
        cmp edx, dword ptr [esi + 0Ch]
        jne before_end_of_file

        push -1
        push offset CDiskFileWin32_ReadPastEnd_0099a960
        lea ecx, dword ptr [esp + 1Ch]
        call CDiskFileWin32_ConstructErrorString_0099a960
        lea eax, dword ptr [esp + 14h]
        push eax
        lea ecx, dword ptr [esp + 14h]
        call CDiskFileWin32_CopyErrorString_0099a960
        lea ecx, dword ptr [esp + 10h]
        call CDiskFileWin32_GetErrorText_0099a960
        lea ecx, dword ptr [esp + 10h]
        call CDiskFileWin32_DestroyErrorString_0099a960
        push offset CDiskFileWin32_ThrowInfo_0099a960
        lea ecx, dword ptr [esp + 8]
        push ecx
        mov dword ptr [esp + 0Ch], 3
        call CDiskFileWin32_ThrowError_0099a960

    before_end_of_file:
        push 0
        lea edx, dword ptr [esp + 0Ch]
        push edx
        push eax
        mov eax, dword ptr [esi + 10h]
        push ecx
        push eax
        call dword ptr [CDiskFileWin32_ReadFileIAT_0099a960]
        cmp eax, 1
        je update_position

        push -1
        push offset CDiskFileWin32_ReadFailure_0099a960
        lea ecx, dword ptr [esp + 1Ch]
        call CDiskFileWin32_ConstructErrorString_0099a960
        lea ecx, dword ptr [esp + 14h]
        push ecx
        lea ecx, dword ptr [esp + 14h]
        call CDiskFileWin32_CopyErrorString_0099a960
        lea ecx, dword ptr [esp + 10h]
        call CDiskFileWin32_GetErrorText_0099a960
        lea ecx, dword ptr [esp + 10h]
        call CDiskFileWin32_DestroyErrorString_0099a960
        push offset CDiskFileWin32_ThrowInfo_0099a960
        lea edx, dword ptr [esp + 8]
        push edx
        mov dword ptr [esp + 0Ch], 4
        call CDiskFileWin32_ThrowError_0099a960

    update_position:
        mov ecx, dword ptr [esi + 8]
        mov eax, dword ptr [esp + 8]
        add ecx, eax
        mov dword ptr [esi + 8], ecx

    read_done:
        pop esi
        add esp, 8
        ret 0Ch
    }
}
