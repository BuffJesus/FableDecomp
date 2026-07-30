#include "fable_config_detection.h"
#include "fable_eula.h"
#include "fable_gfmain.h"

#include <string.h>

void* g_FableConfigDetectionModule_013B7C8C = 0;
FableGetProcAddressFunction g_FableGetProcAddress_0143FE44 = 0;
FableFreeLibraryFunction g_FableFreeLibrary_0143FE3C = 0;
FableLoadLibraryAFunction g_FableLoadLibraryA_0143FE40 = 0;
FableMessageBoxAFunction g_FableMessageBoxA_014402C4 = 0;

extern "C" fable_u32 g_FableConfigSecurityCookie_0139C8A8 =
    0x4F17C0DE;
extern "C" void* g_FableConfigDetectionInfo_013BD6E8 = 0;
extern "C" fable_u8 g_FableConfigRegistryKey_013BCA28 = 0;
extern "C" fable_u8 g_FableConfigRegistryValue_013BCA2C = 0;

extern "C" fable_i32 g_FableConfigMinA_0137507C = -1;
extern "C" fable_i32 g_FableConfigMinB_01375078 = -1;
extern "C" fable_i32 g_FableConfigMinC_01375070 = -1;
extern "C" fable_i32 g_FableConfigMinD_01375074 = -1;
extern "C" fable_i32 g_FableConfigMaxA_01375098 = -1;
extern "C" fable_i32 g_FableConfigMaxB_01375094 = -1;
extern "C" fable_i32 g_FableConfigMaxC_0137508C = -1;
extern "C" fable_i32 g_FableConfigMaxD_01375090 = -1;
extern "C" fable_i32 g_FableConfigLevel_01375080 = -1;
extern "C" fable_u32 g_FableConfigWidth_01375084 = 640;
extern "C" fable_u32 g_FableConfigRefresh_01375088 = 30;
extern "C" fable_u32 g_FableConfigMemoryA_0137509C = 1;
extern "C" fable_u32 g_FableConfigMemoryB_013750A0 = 2;
extern "C" fable_u8 g_FableConfigFlagA_013750A4 = 0;
extern "C" fable_u8 g_FableConfigFlagB_013750A5 = 0;
extern "C" fable_u8 g_FableConfigFlagC_013750A7 = 0;
extern "C" fable_u8 g_FableConfigFlagD_013750A6 = 0;
extern "C" fable_u8 g_FableConfigFlagE_013750A9 = 0;
extern "C" fable_u8 g_FableConfigFlagF_013750AA = 0;
extern "C" fable_u8 g_FableConfigFlagG_013750AB = 0;
extern "C" fable_u8 g_FableConfigUseSoftware_013750AC = 0;

extern "C" fable_u8 g_FableEulaLanguage_013BCA28 = 0;
extern "C" fable_u8 g_FableEulaEmptyText_0129AAF4 = 0;
extern "C" const char* g_FableEulaDocument_01374F54 = "eula.rtf";
extern "C" const char* g_FableWarrantyDocument_01374F58 =
    "warranty.rtf";

namespace
{
    struct FableBoundaryStringData
    {
        const char* text;
    };

    fable_u8 g_ConfigInfo[0xC0];
    FableBoundaryStringData g_EulaLanguage = {"English"};
    GFMainPhase8State* g_ActivePhase8State;

    fable_u32& ConfigInfo(size_t offset)
    {
        return *reinterpret_cast<fable_u32*>(g_ConfigInfo + offset);
    }

    int FABLE_STDCALL EulaExport(
        const char*,
        const char*,
        const char*,
        int)
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactEulaExportCalls;
        return 1;
    }

    void* FABLE_STDCALL EulaLoadLibrary(const unsigned short* name)
    {
        static const unsigned short expected[] = L"eula.dll";
        return
            name != 0 && memcmp(name, expected, sizeof(expected)) == 0
                ? reinterpret_cast<void*>(0xE11A0001)
                : 0;
    }

    void* FABLE_STDCALL EulaGetProcAddress(void*, const char* name)
    {
        return
            name != 0 && strcmp(name, "EBUEula") == 0
                ? reinterpret_cast<void*>(&EulaExport)
                : 0;
    }

    int FABLE_STDCALL EulaFreeLibrary(void*)
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactEulaFreeCalls;
        return 1;
    }

    int FABLE_STDCALL CreateConfigDetection(void** output, void*)
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactConfigurationCreateCalls;
        *output = g_ConfigInfo;
        return 1;
    }

    int FABLE_STDCALL InitialiseVideoCard(int, char*)
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactConfigurationVideoCalls;
        return 1;
    }

    void FABLE_STDCALL CheckRequirements()
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactConfigurationRequirementsCalls;
    }

    void FABLE_STDCALL ReleaseConfiguration()
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactConfigurationReleaseCalls;
    }

    void* FABLE_STDCALL ConfigLoadLibrary(const char* name)
    {
        return
            name != 0 && strcmp(name, "ConfigDetect.dll") == 0
                ? reinterpret_cast<void*>(0xC0F10001)
                : 0;
    }

    void* FABLE_STDCALL ConfigGetProcAddress(void*, const char* name)
    {
        if (name == 0)
            return 0;
        if (strcmp(name, "CreateConfigDetection") == 0)
            return reinterpret_cast<void*>(&CreateConfigDetection);
        if (strcmp(name, "Config_InitVideoCardInfo") == 0)
            return reinterpret_cast<void*>(&InitialiseVideoCard);
        if (strcmp(name, "CheckRequirements") == 0)
            return reinterpret_cast<void*>(&CheckRequirements);
        if (strcmp(name, "Config_Release") == 0)
            return reinterpret_cast<void*>(&ReleaseConfiguration);
        return 0;
    }

    int FABLE_STDCALL ConfigFreeLibrary(void*)
    {
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactConfigurationFreeCalls;
        return 1;
    }

    int FABLE_STDCALL ConfigMessageBox(
        void*,
        const char*,
        const char*,
        fable_u32)
    {
        return 0;
    }

    void PrepareConfigurationInfo()
    {
        memset(g_ConfigInfo, 0, sizeof(g_ConfigInfo));
        ConfigInfo(0x04) = 800;
        ConfigInfo(0x14) = 1;
        ConfigInfo(0x70) = 1;
        ConfigInfo(0x7C) = 1;
        ConfigInfo(0x90) = 3;
        ConfigInfo(0x94) = 8;
        ConfigInfo(0x98) = 2;
        ConfigInfo(0x9C) = 1;
        ConfigInfo(0xA0) = 6;
        ConfigInfo(0xA4) = 3;
        ConfigInfo(0xA8) = 5;
        ConfigInfo(0xAC) = 3;
        ConfigInfo(0xB0) = 9;
        ConfigInfo(0xB8) = 128;
        ConfigInfo(0xBC) = 256;
    }
}

extern "C" void* (FABLE_STDCALL*
    g_FableEulaLoadLibraryW_00401fe0)(const unsigned short*) =
        &EulaLoadLibrary;
extern "C" void* (FABLE_STDCALL*
    g_FableEulaGetProcAddress_00401fe0)(void*, const char*) =
        &EulaGetProcAddress;
extern "C" int (FABLE_STDCALL*
    g_FableEulaFreeLibrary_00401fe0)(void*) =
        &EulaFreeLibrary;

extern "C" void* __fastcall FableEulaConvertLanguage_00401fe0(
    void*,
    void*,
    void* output)
{
    *static_cast<FableBoundaryStringData**>(output) = &g_EulaLanguage;
    return output;
}

extern "C" void __fastcall
FableEulaDestroyLanguage_00401fe0(void*, void*)
{
}

extern "C" void FableConfigDisplayErrorCallback_0099DCB0()
{
}

extern "C" void FableConfigInitExternalStrings_004017b0()
{
}

extern "C" char* __cdecl FableConfigStrncpy_004017b0(
    char* destination,
    const char* source,
    fable_u32 count)
{
    return strncpy(destination, source, count);
}

extern "C" void __fastcall FableConfigRegistryConstruct_004017b0(
    void*,
    void*,
    const void*,
    fable_i32)
{
}

extern "C" bool __fastcall FableConfigRegistryReadBool_004017b0(
    void*,
    void*,
    const void*,
    fable_i32)
{
    return false;
}

extern "C" int FABLE_STDCALL FableConfigDisplayError_004017b0(
    void*,
    fable_i32)
{
    return 0;
}

extern "C" void __fastcall FableConfigRegistryWriteBool_004017b0(
    void*,
    void*,
    const void*,
    fable_i32)
{
}

extern "C" void __fastcall
FableConfigRegistryDestroy_004017b0(void*, void*)
{
}

extern "C" void __fastcall
FableConfigSecurityCheck_004017b0(fable_u32 cookie, void*)
{
    if (
        g_ActivePhase8State != 0 &&
        cookie == g_FableConfigSecurityCookie_0139C8A8)
    {
        ++g_ActivePhase8State->exactConfigurationSecurityChecks;
    }
}

bool FABLE_FASTCALL FableRunExactPhase8EulaBoundary(
    GFMainPhase8State& state)
{
    g_ActivePhase8State = &state;
    const bool result = EULA::DoEULAThings() != 0;
    g_ActivePhase8State = 0;
    return result;
}

bool FABLE_FASTCALL FableRunExactPhase8ConfigurationBoundary(
    GFMainPhase8State& state)
{
    PrepareConfigurationInfo();
    g_FableLoadLibraryA_0143FE40 = &ConfigLoadLibrary;
    g_FableGetProcAddress_0143FE44 = &ConfigGetProcAddress;
    g_FableFreeLibrary_0143FE3C = &ConfigFreeLibrary;
    g_FableMessageBoxA_014402C4 = &ConfigMessageBox;
    g_ActivePhase8State = &state;
    const bool result = GFConfigDetection();
    g_ActivePhase8State = 0;
    return result;
}

void FABLE_FASTCALL FableRunExactPhase8FreeConfigurationBoundary(
    GFMainPhase8State& state)
{
    g_ActivePhase8State = &state;
    GFFreeConfigDetection();
    g_ActivePhase8State = 0;
}
