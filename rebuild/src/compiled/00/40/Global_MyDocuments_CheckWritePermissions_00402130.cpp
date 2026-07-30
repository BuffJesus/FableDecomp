#include "fable_write_permissions.h"

extern "C" fable_u8 g_FableWriteEmptyWide_0122D70C;
extern "C" fable_u8 g_FableWriteEmptyNarrow_0122D70E;
extern "C" fable_u8 g_FableWriteRestrictedKey_0122D710;
extern "C" fable_u8 g_FableWriteOpenMode_0122D744;
extern "C" fable_u8 g_FableWriteProbeSuffix_0122D750;
extern "C" fable_u8 g_FableWriteRedirectedKey_0122D7E4;
extern "C" fable_u8 g_FableWriteRedirectedPrefix_0122D818;
extern "C" fable_u8 g_FableWriteEmptyCharData_0129AAF4;

extern "C" void* g_FableCreateFileW_0143FE2C;
extern "C" void* g_FableCloseHandle_0143FE30;
extern "C" void* g_FableDeleteFileW_0143FE34;

extern "C" void FABLE_FASTCALL
FableWriteCharConstruct_00402130(void* string, void*);
extern "C" void* FABLE_FASTCALL
FableWriteWideSubstring_00402130(
    const void* source,
    void*,
    void* result,
    fable_i32 start,
    fable_i32 count);
extern "C" fable_i32 FABLE_CDECL
FableWriteWideCompare_00402130(
    const wchar_t* left,
    const wchar_t* right);
extern "C" fable_i32 FABLE_CDECL
FableWriteWideMemoryCompare_00402130(
    const wchar_t* left,
    const wchar_t* right,
    fable_i32 count);
extern "C" void FABLE_FASTCALL
FableWriteWideDestroy_00402130(void* string, void*);
extern "C" void* FABLE_FASTCALL
FableWriteCharAssignLiteral_00402130(
    void* string,
    void*,
    const char* literal);
extern "C" void FABLE_FASTCALL
FableWriteWideAppendLiteral_00402130(
    void* result,
    const void* left,
    const wchar_t* right);
extern "C" const wchar_t* FABLE_FASTCALL
FableWriteWideConversion_00402130(const void* string, void*);
extern "C" void* FABLE_CDECL
FableWriteWideFileOpen_00402130(
    const wchar_t* filename,
    const wchar_t* mode);
extern "C" fable_i32 FABLE_CDECL
FableWriteFileClose_00402130(void* stream);
extern "C" bool FABLE_FASTCALL
FableWriteCharNotEqual_00402130(
    const void* string,
    void*,
    const char* literal);
extern "C" fable_u32 FABLE_FASTCALL
FableWriteCrc_00402130(
    fable_u32 seed,
    const void* data,
    fable_u32 length);
extern "C" void FABLE_FASTCALL
FableWriteCharDestroy_00402130(void* string, void*);

// GFMain's My Documents write-permission probe @ 0x00402130.
__declspec(naked) fable_u32 FABLE_FASTCALL
FableCheckMyDocumentsWritePermissions_00402130(const CWideString&)
{
    __asm
    {
        sub esp, 8
        push ebx
        push esi
        mov esi, ecx
        lea ecx, dword ptr [esp + 8]
        call FableWriteCharConstruct_00402130
        push 2
        push 0
        lea eax, dword ptr [esp + 14h]
        push eax
        mov ecx, esi
        call FableWriteWideSubstring_00402130
        mov ecx, dword ptr [eax]
        test ecx, ecx
        jnz compare_prefix_length
        push offset g_FableWriteEmptyWide_0122D70C
        push offset g_FableWriteRedirectedPrefix_0122D818
        call FableWriteWideCompare_00402130
        add esp, 8
        mov ebx, eax
        neg ebx
        sbb bl, bl
        inc bl
        jmp prefix_checked

    compare_prefix_length:
        xor eax, eax

    count_prefix:
        inc eax
        cmp word ptr [eax * 2 + g_FableWriteRedirectedPrefix_0122D818], 0
        jnz count_prefix
        mov edx, dword ptr [ecx]
        mov ecx, dword ptr [ecx + 4]
        sub ecx, edx
        sar ecx, 1
        cmp ecx, eax
        jnz not_redirected
        push eax
        push offset g_FableWriteRedirectedPrefix_0122D818
        push edx
        call FableWriteWideMemoryCompare_00402130
        add esp, 0Ch
        test eax, eax
        jnz not_redirected
        mov bl, 1
        jmp prefix_checked

    not_redirected:
        xor bl, bl

    prefix_checked:
        lea ecx, dword ptr [esp + 0Ch]
        call FableWriteWideDestroy_00402130
        test bl, bl
        jz probe_path
        push offset g_FableWriteRedirectedKey_0122D7E4
        lea ecx, dword ptr [esp + 0Ch]
        call FableWriteCharAssignLiteral_00402130
        jmp evaluate_error

    probe_path:
        push offset g_FableWriteProbeSuffix_0122D750
        mov edx, esi
        lea ecx, dword ptr [esp + 10h]
        call FableWriteWideAppendLiteral_00402130
        push 0
        push 0
        push 2
        push 0
        push 3
        push 10000000h
        lea ecx, dword ptr [esp + 24h]
        call FableWriteWideConversion_00402130
        push eax
        call dword ptr [g_FableCreateFileW_0143FE2C]
        cmp eax, -1
        jz restricted
        push eax
        call dword ptr [g_FableCloseHandle_0143FE30]
        push offset g_FableWriteOpenMode_0122D744
        lea ecx, dword ptr [esp + 10h]
        call FableWriteWideConversion_00402130
        push eax
        call FableWriteWideFileOpen_00402130
        add esp, 8
        test eax, eax
        jz restricted
        push eax
        call FableWriteFileClose_00402130
        add esp, 4
        lea ecx, dword ptr [esp + 0Ch]
        call FableWriteWideConversion_00402130
        push eax
        call dword ptr [g_FableDeleteFileW_0143FE34]
        test eax, eax
        jnz probe_finished

    restricted:
        lea ecx, dword ptr [esp + 8]
        push offset g_FableWriteRestrictedKey_0122D710
        call FableWriteCharAssignLiteral_00402130

    probe_finished:
        lea ecx, dword ptr [esp + 0Ch]
        call FableWriteWideDestroy_00402130

    evaluate_error:
        push offset g_FableWriteEmptyNarrow_0122D70E
        lea ecx, dword ptr [esp + 0Ch]
        call FableWriteCharNotEqual_00402130
        test al, al
        jz return_success
        mov eax, dword ptr [esp + 8]
        test eax, eax
        jnz use_error_data
        xor ecx, ecx
        mov edx, offset g_FableWriteEmptyCharData_0129AAF4
        jmp calculate_crc

    use_error_data:
        mov ecx, dword ptr [eax + 4]
        mov edx, dword ptr [eax]

    calculate_crc:
        push ecx
        xor ecx, ecx
        call FableWriteCrc_00402130
        lea ecx, dword ptr [esp + 8]
        mov esi, eax
        call FableWriteCharDestroy_00402130
        mov eax, esi
        pop esi
        pop ebx
        add esp, 8
        ret

    return_success:
        lea ecx, dword ptr [esp + 8]
        call FableWriteCharDestroy_00402130
        pop esi
        xor eax, eax
        pop ebx
        add esp, 8
        ret
    }
}
