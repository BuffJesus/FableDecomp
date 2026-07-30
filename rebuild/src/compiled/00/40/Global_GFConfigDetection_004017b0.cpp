#include "fable_config_detection.h"

namespace
{
    const char kConfigLibrary[] = "ConfigDetect.dll";
    const char kCreateExport[] = "CreateConfigDetection";
    const char kVideoExport[] = "Config_InitVideoCardInfo";
    const char kUnspecified[] = "unspecified";
    const char kRequirementsExport[] = "CheckRequirements";
    const char kConfigError[] = "ConfigDetection Error";
}

extern "C" fable_u32 g_FableConfigSecurityCookie_0139C8A8;
extern "C" void* g_FableConfigDetectionInfo_013BD6E8;
extern "C" fable_u8 g_FableConfigRegistryKey_013BCA28;
extern "C" fable_u8 g_FableConfigRegistryValue_013BCA2C;

extern "C" fable_i32 g_FableConfigMinA_0137507C;
extern "C" fable_i32 g_FableConfigMinB_01375078;
extern "C" fable_i32 g_FableConfigMinC_01375070;
extern "C" fable_i32 g_FableConfigMinD_01375074;
extern "C" fable_i32 g_FableConfigMaxA_01375098;
extern "C" fable_i32 g_FableConfigMaxB_01375094;
extern "C" fable_i32 g_FableConfigMaxC_0137508C;
extern "C" fable_i32 g_FableConfigMaxD_01375090;
extern "C" fable_i32 g_FableConfigLevel_01375080;
extern "C" fable_u32 g_FableConfigWidth_01375084;
extern "C" fable_u32 g_FableConfigRefresh_01375088;
extern "C" fable_u32 g_FableConfigMemoryA_0137509C;
extern "C" fable_u32 g_FableConfigMemoryB_013750A0;
extern "C" fable_u8 g_FableConfigFlagA_013750A4;
extern "C" fable_u8 g_FableConfigFlagB_013750A5;
extern "C" fable_u8 g_FableConfigFlagC_013750A7;
extern "C" fable_u8 g_FableConfigFlagD_013750A6;
extern "C" fable_u8 g_FableConfigFlagE_013750A9;
extern "C" fable_u8 g_FableConfigFlagF_013750AA;
extern "C" fable_u8 g_FableConfigFlagG_013750AB;
extern "C" fable_u8 g_FableConfigUseSoftware_013750AC;

extern "C" void FableConfigDisplayErrorCallback_0099DCB0();
extern "C" void FableConfigInitExternalStrings_004017b0();
extern "C" char* __cdecl FableConfigStrncpy_004017b0(
    char*,
    const char*,
    fable_u32);
extern "C" void __fastcall FableConfigRegistryConstruct_004017b0(
    void*,
    void*,
    const void*,
    fable_i32);
extern "C" bool __fastcall FableConfigRegistryReadBool_004017b0(
    void*,
    void*,
    const void*,
    fable_i32);
extern "C" int FABLE_STDCALL FableConfigDisplayError_004017b0(
    void*,
    fable_i32);
extern "C" void __fastcall FableConfigRegistryWriteBool_004017b0(
    void*,
    void*,
    const void*,
    fable_i32);
extern "C" void __fastcall FableConfigRegistryDestroy_004017b0(void*, void*);
extern "C" void __fastcall FableConfigSecurityCheck_004017b0(
    fable_u32,
    void*);

__declspec(naked) bool FABLE_FASTCALL GFConfigDetection()
{
    __asm
    {
        sub esp, 118h
        mov eax, dword ptr [g_FableConfigSecurityCookie_0139C8A8]
        push ebx
        mov dword ptr [esp + 118h], eax
        call FableConfigInitExternalStrings_004017b0
        push offset kConfigLibrary
        call dword ptr [g_FableLoadLibraryA_0143FE40]
        xor ebx, ebx
        cmp eax, ebx
        mov dword ptr [g_FableConfigDetectionModule_013B7C8C], eax
        jnz module_loaded
        xor al, al
        pop ebx
        mov ecx, dword ptr [esp + 114h]
        call FableConfigSecurityCheck_004017b0
        add esp, 118h
        ret

    module_loaded:
        push esi
        mov esi, dword ptr [g_FableGetProcAddress_0143FE44]
        push offset kCreateExport
        push eax
        call esi
        cmp eax, ebx
        jnz create_export_ready

    early_failure:
        pop esi
        xor al, al
        pop ebx
        mov ecx, dword ptr [esp + 114h]
        call FableConfigSecurityCheck_004017b0
        add esp, 118h
        ret

    create_export_ready:
        push offset FableConfigDisplayErrorCallback_0099DCB0
        push offset g_FableConfigDetectionInfo_013BD6E8
        call eax
        test eax, eax
        jz early_failure
        mov eax, dword ptr [g_FableConfigDetectionModule_013B7C8C]
        push edi
        push offset kVideoExport
        push eax
        call esi
        mov edi, eax
        cmp edi, ebx
        jz final_failure
        push 103h
        lea ecx, dword ptr [esp + 20h]
        push offset kUnspecified
        push ecx
        call FableConfigStrncpy_004017b0
        add esp, 0Ch
        lea edx, dword ptr [esp + 1Ch]
        push edx
        push ebx
        call edi
        test eax, eax
        jz video_failure
        mov eax, dword ptr [g_FableConfigDetectionModule_013B7C8C]
        push offset kRequirementsExport
        push eax
        call esi
        cmp eax, ebx
        jz requirements_done
        call eax

    requirements_done:
        mov eax, dword ptr [g_FableConfigDetectionInfo_013BD6E8]
        cmp dword ptr [eax + 70h], ebx
        setnz cl
        mov byte ptr [g_FableConfigFlagA_013750A4], cl
        cmp dword ptr [eax + 74h], ebx
        setnz dl
        mov byte ptr [g_FableConfigFlagB_013750A5], dl
        cmp dword ptr [eax + 78h], ebx
        setz cl
        mov byte ptr [g_FableConfigFlagC_013750A7], cl
        cmp dword ptr [eax + 7Ch], ebx
        setnz dl
        mov byte ptr [g_FableConfigFlagD_013750A6], dl
        cmp dword ptr [eax + 88h], ebx
        setz cl
        mov byte ptr [g_FableConfigFlagE_013750A9], cl
        cmp dword ptr [eax + 8Ch], ebx
        setz dl
        mov byte ptr [g_FableConfigFlagF_013750AA], dl
        cmp dword ptr [eax + 14h], ebx
        setnz cl
        mov byte ptr [g_FableConfigFlagG_013750AB], cl
        mov edx, dword ptr [eax + 90h]
        dec edx
        mov dword ptr [g_FableConfigMinA_0137507C], edx
        mov ecx, dword ptr [eax + 94h]
        dec ecx
        mov dword ptr [g_FableConfigMinB_01375078], ecx
        mov edx, dword ptr [eax + 98h]
        dec edx
        mov dword ptr [g_FableConfigMinC_01375070], edx
        mov ecx, dword ptr [eax + 9Ch]
        dec ecx
        mov dword ptr [g_FableConfigMinD_01375074], ecx
        mov edx, dword ptr [eax + 0A0h]
        dec edx
        mov dword ptr [g_FableConfigMaxA_01375098], edx
        mov ecx, dword ptr [eax + 0A4h]
        dec ecx
        mov dword ptr [g_FableConfigMaxB_01375094], ecx
        mov edx, dword ptr [eax + 0A8h]
        dec edx
        mov dword ptr [g_FableConfigMaxC_0137508C], edx
        mov ecx, dword ptr [eax + 0ACh]
        dec ecx
        mov dword ptr [g_FableConfigMaxD_01375090], ecx
        mov edx, dword ptr [eax + 0B0h]
        dec edx
        mov dword ptr [g_FableConfigLevel_01375080], edx
        mov ecx, dword ptr [eax + 4]
        cmp ecx, ebx
        jbe width_ready
        mov dword ptr [g_FableConfigWidth_01375084], ecx

    width_ready:
        mov ecx, dword ptr [eax + 0B8h]
        cmp ecx, ebx
        jbe memory_a_ready
        mov dword ptr [g_FableConfigMemoryA_0137509C], ecx

    memory_a_ready:
        mov eax, dword ptr [eax + 0BCh]
        cmp eax, ebx
        jbe memory_b_ready
        mov dword ptr [g_FableConfigMemoryB_013750A0], eax

    memory_b_ready:
        push ebx
        push offset g_FableConfigRegistryKey_013BCA28
        lea ecx, dword ptr [esp + 14h]
        call FableConfigRegistryConstruct_004017b0
        push ebx
        push offset g_FableConfigRegistryValue_013BCA2C
        lea ecx, dword ptr [esp + 14h]
        call FableConfigRegistryReadBool_004017b0
        mov ecx, dword ptr [g_FableConfigDetectionInfo_013BD6E8]
        cmp dword ptr [ecx + 58h], ebx
        setnz cl
        cmp cl, bl
        mov byte ptr [g_FableConfigUseSoftware_013750AC], cl
        jnz software_fallback
        cmp eax, ebx
        jz software_fallback
        push ebx
        lea edx, dword ptr [esp + 18h]
        push edx
        mov dword ptr [esp + 1Ch], 801h
        mov dword ptr [esp + 20h], ebx
        call FableConfigDisplayError_004017b0
        cmp eax, 1
        jnz software_fallback
        mov byte ptr [g_FableConfigUseSoftware_013750AC], al

    software_fallback:
        push 1
        push offset g_FableConfigRegistryValue_013BCA2C
        lea ecx, dword ptr [esp + 14h]
        call FableConfigRegistryWriteBool_004017b0
        cmp byte ptr [g_FableConfigUseSoftware_013750AC], bl
        jz clamp_ranges
        lea ecx, dword ptr [esp + 0Ch]
        mov dword ptr [g_FableConfigMinA_0137507C], ebx
        mov dword ptr [g_FableConfigMinB_01375078], ebx
        mov dword ptr [g_FableConfigMinC_01375070], ebx
        mov dword ptr [g_FableConfigMinD_01375074], ebx
        mov dword ptr [g_FableConfigMaxA_01375098], ebx
        mov dword ptr [g_FableConfigMaxB_01375094], ebx
        mov dword ptr [g_FableConfigMaxC_0137508C], ebx
        mov dword ptr [g_FableConfigMaxD_01375090], ebx
        mov dword ptr [g_FableConfigLevel_01375080], ebx
        mov dword ptr [g_FableConfigWidth_01375084], 400h
        mov dword ptr [g_FableConfigRefresh_01375088], 3Ch
        call FableConfigRegistryDestroy_004017b0
        pop edi
        pop esi
        mov al, 1
        pop ebx
        mov ecx, dword ptr [esp + 114h]
        call FableConfigSecurityCheck_004017b0
        add esp, 118h
        ret

    clamp_ranges:
        mov ecx, dword ptr [g_FableConfigMinA_0137507C]
        cmp ecx, ebx
        jl clamp_b
        mov eax, dword ptr [g_FableConfigMaxA_01375098]
        cmp eax, ebx
        jl clamp_b
        cmp ecx, eax
        jge clamp_b
        mov dword ptr [g_FableConfigMaxA_01375098], ecx

    clamp_b:
        mov ecx, dword ptr [g_FableConfigMinB_01375078]
        cmp ecx, ebx
        jl clamp_c
        mov eax, dword ptr [g_FableConfigMaxB_01375094]
        cmp eax, ebx
        jl clamp_c
        cmp ecx, eax
        jge clamp_c
        mov dword ptr [g_FableConfigMaxB_01375094], ecx

    clamp_c:
        mov ecx, dword ptr [g_FableConfigMinC_01375070]
        cmp ecx, ebx
        jl clamp_d
        mov eax, dword ptr [g_FableConfigMaxC_0137508C]
        cmp eax, ebx
        jl clamp_d
        cmp ecx, eax
        jge clamp_d
        mov dword ptr [g_FableConfigMaxC_0137508C], ecx

    clamp_d:
        mov ecx, dword ptr [g_FableConfigMinD_01375074]
        cmp ecx, ebx
        jl normal_success
        mov eax, dword ptr [g_FableConfigMaxD_01375090]
        cmp eax, ebx
        jl normal_success
        cmp ecx, eax
        jge normal_success
        mov dword ptr [g_FableConfigMaxD_01375090], ecx

    normal_success:
        lea ecx, dword ptr [esp + 0Ch]
        call FableConfigRegistryDestroy_004017b0
        pop edi
        pop esi
        mov al, 1
        pop ebx
        mov ecx, dword ptr [esp + 114h]
        call FableConfigSecurityCheck_004017b0
        add esp, 118h
        ret

    video_failure:
        push ebx
        push offset kConfigError
        lea eax, dword ptr [esp + 24h]
        push eax
        push ebx
        call dword ptr [g_FableMessageBoxA_014402C4]

    final_failure:
        mov ecx, dword ptr [esp + 120h]
        pop edi
        pop esi
        xor al, al
        pop ebx
        call FableConfigSecurityCheck_004017b0
        add esp, 118h
        ret
    }
}
