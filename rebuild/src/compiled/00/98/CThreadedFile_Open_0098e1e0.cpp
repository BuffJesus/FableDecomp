#include "fable_threaded_file.h"

extern "C" fable_u8 g_FableThreadedEmptyChar_0122D70E;
extern "C" fable_u8 g_FableThreadedDDrive_0129A15C;
extern "C" fable_u32 g_FableThreadedPhysicalSortKey_013BC9EC;
extern "C" void* g_FableThreadedCreateFileW_0143FE2C;
extern "C" void* g_FableThreadedGetFileSize_0143FDF0;

extern "C" void FABLE_FASTCALL
FableThreadedConvertFullPath_0098E1E0(
    void* result,
    const void* pathname);
extern "C" void FABLE_FASTCALL
FableThreadedCharConstruct_0098E1E0(void* string, void*);
extern "C" void FABLE_FASTCALL
FableThreadedSplitPath_0098E1E0(
    const void* fullPath,
    void* drive,
    void* directory,
    void* filename,
    void* extension);
extern "C" void* FABLE_FASTCALL
FableThreadedConvertDriveToChar_0098E1E0(
    void* temporary,
    void*,
    void* drive,
    void* result);
extern "C" void* FABLE_FASTCALL
FableThreadedCharToUpper_0098E1E0(void* string, void*);
extern "C" bool FABLE_FASTCALL
FableThreadedCharEquals_0098E1E0(
    const void* string,
    void*,
    const char* literal);
extern "C" void FABLE_FASTCALL
FableThreadedCharDestroy_0098E1E0(void* string, void*);
extern "C" const wchar_t* FABLE_FASTCALL
FableThreadedWideConversion_0098E1E0(const void* string, void*);
extern "C" void FABLE_FASTCALL
FableThreadedWideAssign_0098E1E0(
    void* destination,
    void*,
    const void* source);

// CThreadedFile::Open(CWideString const&, bool) @ 0x0098E1E0.
__declspec(naked) bool
CThreadedFile::Open(const CWideString&, bool)
{
    __asm
    {
        mov edx, dword ptr [esp + 4]
        sub esp, 10h
        push ebx
        push ebp
        mov ebp, ecx
        lea ecx, dword ptr [esp + 0Ch]
        call FableThreadedConvertFullPath_0098E1E0
        lea ecx, dword ptr [esp + 8]
        call FableThreadedCharConstruct_0098E1E0
        push 0
        push 0
        push 0
        lea edx, dword ptr [esp + 14h]
        lea ecx, dword ptr [esp + 18h]
        call FableThreadedSplitPath_0098E1E0
        lea eax, dword ptr [esp + 10h]
        push eax
        lea ecx, dword ptr [esp + 18h]
        push ecx
        lea ecx, dword ptr [esp + 10h]
        mov byte ptr [ebp + 18h], 0
        call FableThreadedConvertDriveToChar_0098E1E0
        mov ecx, eax
        call FableThreadedCharToUpper_0098E1E0
        mov ecx, dword ptr [eax]
        test ecx, ecx
        jnz compare_drive
        push esi
        push edi
        mov edi, offset g_FableThreadedEmptyChar_0122D70E
        mov esi, offset g_FableThreadedDDrive_0129A15C
        mov ecx, 3
        xor edx, edx
        repe cmpsb
        pop edi
        sete bl
        pop esi
        jmp drive_checked

    compare_drive:
        push offset g_FableThreadedDDrive_0129A15C
        call FableThreadedCharEquals_0098E1E0
        mov bl, al

    drive_checked:
        lea ecx, dword ptr [esp + 10h]
        call FableThreadedCharDestroy_0098E1E0
        lea ecx, dword ptr [esp + 14h]
        call FableThreadedCharDestroy_0098E1E0
        mov cl, byte ptr [esp + 20h]
        xor eax, eax
        test bl, bl
        setne al
        test cl, cl
        mov dword ptr [ebp + 4], eax
        mov eax, 40000001h
        jz select_flags_done
        mov eax, 60000001h

    select_flags_done:
        push 0
        push eax
        push 3
        push 0
        push 1
        push 80000000h
        lea ecx, dword ptr [esp + 24h]
        call FableThreadedWideConversion_0098E1E0
        push eax
        call dword ptr [g_FableThreadedCreateFileW_0143FE2C]
        cmp eax, -1
        mov dword ptr [ebp + 8], eax
        jnz opened
        lea ecx, dword ptr [esp + 8]
        call FableThreadedCharDestroy_0098E1E0
        lea ecx, dword ptr [esp + 0Ch]
        call FableThreadedCharDestroy_0098E1E0
        pop ebp
        xor al, al
        pop ebx
        add esp, 10h
        ret 8

    opened:
        mov ecx, dword ptr [esp + 1Ch]
        push ecx
        lea ecx, dword ptr [ebp + 0Ch]
        call FableThreadedWideAssign_0098E1E0
        mov edx, dword ptr [ebp + 8]
        push 0
        push edx
        call dword ptr [g_FableThreadedGetFileSize_0143FDF0]
        mov dword ptr [ebp + 10h], eax
        mov eax, dword ptr [g_FableThreadedPhysicalSortKey_013BC9EC]
        inc eax
        lea ecx, dword ptr [esp + 8]
        mov dword ptr [g_FableThreadedPhysicalSortKey_013BC9EC], eax
        mov dword ptr [ebp + 14h], eax
        mov byte ptr [ebp + 19h], 1
        call FableThreadedCharDestroy_0098E1E0
        lea ecx, dword ptr [esp + 0Ch]
        call FableThreadedCharDestroy_0098E1E0
        pop ebp
        mov al, 1
        pop ebx
        add esp, 10h
        ret 8
    }
}
