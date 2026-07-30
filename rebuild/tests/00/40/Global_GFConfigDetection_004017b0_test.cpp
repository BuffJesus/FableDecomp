#include <stdio.h>
#include <string.h>

#include "fable_config_detection.h"

void* g_FableConfigDetectionModule_013B7C8C = 0;
FableGetProcAddressFunction g_FableGetProcAddress_0143FE44 = 0;
FableLoadLibraryAFunction g_FableLoadLibraryA_0143FE40 = 0;
FableMessageBoxAFunction g_FableMessageBoxA_014402C4 = 0;

extern "C" fable_u32 g_FableConfigSecurityCookie_0139C8A8 = 0x12345678;
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

namespace
{
    unsigned char g_info[0xC0];
    bool g_loadSucceeds;
    bool g_createPresent;
    bool g_createSucceeds;
    bool g_videoPresent;
    bool g_videoSucceeds;
    bool g_requirementsPresent;
    bool g_registryRead;
    int g_dialogResult;
    fable_u32 g_initStringsCalls;
    fable_u32 g_loadCalls;
    fable_u32 g_getProcCalls;
    fable_u32 g_createCalls;
    fable_u32 g_videoCalls;
    fable_u32 g_requirementsCalls;
    fable_u32 g_messageCalls;
    fable_u32 g_registryConstructCalls;
    fable_u32 g_registryReadCalls;
    fable_u32 g_registryWriteCalls;
    fable_u32 g_registryDestroyCalls;
    fable_u32 g_dialogCalls;
    fable_u32 g_securityCalls;
    fable_u32 g_observedDialogCode;
    bool g_observedWrite;
    char g_observedMessage[64];

    fable_u32& Info(size_t offset)
    {
        return *reinterpret_cast<fable_u32*>(g_info + offset);
    }

    void Reset()
    {
        memset(g_info, 0, sizeof(g_info));
        g_FableConfigDetectionInfo_013BD6E8 = g_info;
        g_FableConfigDetectionModule_013B7C8C = 0;
        g_loadSucceeds = true;
        g_createPresent = true;
        g_createSucceeds = true;
        g_videoPresent = true;
        g_videoSucceeds = true;
        g_requirementsPresent = true;
        g_registryRead = false;
        g_dialogResult = 0;
        g_initStringsCalls = 0;
        g_loadCalls = 0;
        g_getProcCalls = 0;
        g_createCalls = 0;
        g_videoCalls = 0;
        g_requirementsCalls = 0;
        g_messageCalls = 0;
        g_registryConstructCalls = 0;
        g_registryReadCalls = 0;
        g_registryWriteCalls = 0;
        g_registryDestroyCalls = 0;
        g_dialogCalls = 0;
        g_securityCalls = 0;
        g_observedDialogCode = 0;
        g_observedWrite = false;
        g_observedMessage[0] = '\0';

        g_FableConfigMinA_0137507C = -1;
        g_FableConfigMinB_01375078 = -1;
        g_FableConfigMinC_01375070 = -1;
        g_FableConfigMinD_01375074 = -1;
        g_FableConfigMaxA_01375098 = -1;
        g_FableConfigMaxB_01375094 = -1;
        g_FableConfigMaxC_0137508C = -1;
        g_FableConfigMaxD_01375090 = -1;
        g_FableConfigLevel_01375080 = -1;
        g_FableConfigWidth_01375084 = 640;
        g_FableConfigRefresh_01375088 = 30;
        g_FableConfigMemoryA_0137509C = 1;
        g_FableConfigMemoryB_013750A0 = 2;
        g_FableConfigUseSoftware_013750AC = 0;

        Info(0x04) = 800;
        Info(0x14) = 1;
        Info(0x70) = 1;
        Info(0x74) = 0;
        Info(0x78) = 0;
        Info(0x7C) = 1;
        Info(0x88) = 0;
        Info(0x8C) = 1;
        Info(0x90) = 3;
        Info(0x94) = 8;
        Info(0x98) = 2;
        Info(0x9C) = 1;
        Info(0xA0) = 6;
        Info(0xA4) = 3;
        Info(0xA8) = 5;
        Info(0xAC) = 3;
        Info(0xB0) = 9;
        Info(0xB8) = 128;
        Info(0xBC) = 256;
    }

    int FABLE_STDCALL CreateConfigDetection(
        void** output,
        void*)
    {
        ++g_createCalls;
        *output = g_info;
        return g_createSucceeds ? 1 : 0;
    }

    int FABLE_STDCALL InitialiseVideoCard(int, char* error)
    {
        ++g_videoCalls;
        if (!g_videoSucceeds)
            strcpy(error, "video failed");
        return g_videoSucceeds ? 1 : 0;
    }

    void FABLE_STDCALL CheckRequirements()
    {
        ++g_requirementsCalls;
    }

    void* FABLE_STDCALL LoadLibraryAStub(const char* name)
    {
        ++g_loadCalls;
        return
            g_loadSucceeds && strcmp(name, "ConfigDetect.dll") == 0
                ? reinterpret_cast<void*>(0x11112222)
                : 0;
    }

    void* FABLE_STDCALL GetProcAddressStub(void*, const char* name)
    {
        ++g_getProcCalls;
        if (strcmp(name, "CreateConfigDetection") == 0)
            return g_createPresent
                ? reinterpret_cast<void*>(&CreateConfigDetection)
                : 0;
        if (strcmp(name, "Config_InitVideoCardInfo") == 0)
            return g_videoPresent
                ? reinterpret_cast<void*>(&InitialiseVideoCard)
                : 0;
        if (strcmp(name, "CheckRequirements") == 0)
            return g_requirementsPresent
                ? reinterpret_cast<void*>(&CheckRequirements)
                : 0;
        return 0;
    }

    int FABLE_STDCALL MessageBoxAStub(
        void*,
        const char* text,
        const char*,
        fable_u32)
    {
        ++g_messageCalls;
        strncpy(g_observedMessage, text, sizeof(g_observedMessage) - 1);
        g_observedMessage[sizeof(g_observedMessage) - 1] = '\0';
        return 0;
    }

    bool CheckLoadFailure()
    {
        Reset();
        g_loadSucceeds = false;
        return !GFConfigDetection() &&
            g_initStringsCalls == 1 &&
            g_loadCalls == 1 &&
            g_createCalls == 0 &&
            g_securityCalls == 1;
    }

    bool CheckMissingCreate()
    {
        Reset();
        g_createPresent = false;
        return !GFConfigDetection() &&
            g_getProcCalls == 1 &&
            g_createCalls == 0 &&
            g_securityCalls == 1;
    }

    bool CheckCreateFailure()
    {
        Reset();
        g_createSucceeds = false;
        return !GFConfigDetection() &&
            g_createCalls == 1 &&
            g_videoCalls == 0 &&
            g_securityCalls == 1;
    }

    bool CheckMissingVideo()
    {
        Reset();
        g_videoPresent = false;
        return !GFConfigDetection() &&
            g_createCalls == 1 &&
            g_videoCalls == 0 &&
            g_messageCalls == 0 &&
            g_securityCalls == 1;
    }

    bool CheckVideoFailure()
    {
        Reset();
        g_videoSucceeds = false;
        return !GFConfigDetection() &&
            g_videoCalls == 1 &&
            g_messageCalls == 1 &&
            strcmp(g_observedMessage, "video failed") == 0 &&
            g_securityCalls == 1;
    }

    bool CheckNormalSuccessAndClamping()
    {
        Reset();
        const bool result = GFConfigDetection();
        return result &&
            g_requirementsCalls == 1 &&
            g_FableConfigFlagA_013750A4 == 1 &&
            g_FableConfigFlagB_013750A5 == 0 &&
            g_FableConfigFlagC_013750A7 == 1 &&
            g_FableConfigFlagD_013750A6 == 1 &&
            g_FableConfigFlagE_013750A9 == 1 &&
            g_FableConfigFlagF_013750AA == 0 &&
            g_FableConfigFlagG_013750AB == 1 &&
            g_FableConfigMinA_0137507C == 2 &&
            g_FableConfigMaxA_01375098 == 2 &&
            g_FableConfigMinB_01375078 == 7 &&
            g_FableConfigMaxB_01375094 == 2 &&
            g_FableConfigMinC_01375070 == 1 &&
            g_FableConfigMaxC_0137508C == 1 &&
            g_FableConfigMinD_01375074 == 0 &&
            g_FableConfigMaxD_01375090 == 0 &&
            g_FableConfigLevel_01375080 == 8 &&
            g_FableConfigWidth_01375084 == 800 &&
            g_FableConfigMemoryA_0137509C == 128 &&
            g_FableConfigMemoryB_013750A0 == 256 &&
            g_registryConstructCalls == 1 &&
            g_registryReadCalls == 1 &&
            g_registryWriteCalls == 1 &&
            g_registryDestroyCalls == 1 &&
            g_observedWrite &&
            g_dialogCalls == 0 &&
            g_securityCalls == 1;
    }

    bool CheckPromptAcceptedFallback()
    {
        Reset();
        g_registryRead = true;
        g_dialogResult = 1;
        const bool result = GFConfigDetection();
        return result &&
            g_dialogCalls == 1 &&
            g_observedDialogCode == 0x801 &&
            g_FableConfigUseSoftware_013750AC == 1 &&
            g_FableConfigMinA_0137507C == 0 &&
            g_FableConfigMaxA_01375098 == 0 &&
            g_FableConfigLevel_01375080 == 0 &&
            g_FableConfigWidth_01375084 == 1024 &&
            g_FableConfigRefresh_01375088 == 60 &&
            g_registryDestroyCalls == 1 &&
            g_securityCalls == 1;
    }

    bool CheckDetectedSoftwareFallback()
    {
        Reset();
        Info(0x58) = 1;
        g_registryRead = true;
        const bool result = GFConfigDetection();
        return result &&
            g_dialogCalls == 0 &&
            g_FableConfigUseSoftware_013750AC == 1 &&
            g_FableConfigWidth_01375084 == 1024 &&
            g_FableConfigRefresh_01375088 == 60;
    }
}

extern "C" void FableConfigDisplayErrorCallback_0099DCB0() {}

extern "C" void FableConfigInitExternalStrings_004017b0()
{
    ++g_initStringsCalls;
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
    ++g_registryConstructCalls;
}

extern "C" fable_u32 __fastcall FableConfigRegistryReadBool_004017b0(
    void*,
    void*,
    const void*,
    fable_i32)
{
    ++g_registryReadCalls;
    return g_registryRead ? 1U : 0U;
}

extern "C" int FABLE_STDCALL FableConfigDisplayError_004017b0(
    void* error,
    fable_i32)
{
    ++g_dialogCalls;
    g_observedDialogCode = *reinterpret_cast<fable_u32*>(error);
    return g_dialogResult;
}

extern "C" void __fastcall FableConfigRegistryWriteBool_004017b0(
    void*,
    void*,
    const void*,
    fable_i32 value)
{
    ++g_registryWriteCalls;
    g_observedWrite = value != 0;
}

extern "C" void __fastcall
FableConfigRegistryDestroy_004017b0(void*, void*)
{
    ++g_registryDestroyCalls;
}

extern "C" void __fastcall
FableConfigSecurityCheck_004017b0(fable_u32 cookie, void*)
{
    if (cookie == g_FableConfigSecurityCookie_0139C8A8)
        ++g_securityCalls;
}

int main()
{
    g_FableLoadLibraryA_0143FE40 = &LoadLibraryAStub;
    g_FableGetProcAddress_0143FE44 = &GetProcAddressStub;
    g_FableMessageBoxA_014402C4 = &MessageBoxAStub;

    const bool load = CheckLoadFailure();
    const bool createMissing = CheckMissingCreate();
    const bool createFailure = CheckCreateFailure();
    const bool videoMissing = CheckMissingVideo();
    const bool videoFailure = CheckVideoFailure();
    const bool normal = CheckNormalSuccessAndClamping();
    if (!normal)
    {
        printf(
            "normal detail flags=%u,%u,%u,%u,%u,%u,%u "
            "mins=%d,%d,%d,%d maxs=%d,%d,%d,%d level=%d "
            "width=%u mem=%u,%u registry=%u,%u,%u,%u write=%u "
            "dialog=%u security=%u req=%u\n",
            g_FableConfigFlagA_013750A4,
            g_FableConfigFlagB_013750A5,
            g_FableConfigFlagC_013750A7,
            g_FableConfigFlagD_013750A6,
            g_FableConfigFlagE_013750A9,
            g_FableConfigFlagF_013750AA,
            g_FableConfigFlagG_013750AB,
            g_FableConfigMinA_0137507C,
            g_FableConfigMinB_01375078,
            g_FableConfigMinC_01375070,
            g_FableConfigMinD_01375074,
            g_FableConfigMaxA_01375098,
            g_FableConfigMaxB_01375094,
            g_FableConfigMaxC_0137508C,
            g_FableConfigMaxD_01375090,
            g_FableConfigLevel_01375080,
            g_FableConfigWidth_01375084,
            g_FableConfigMemoryA_0137509C,
            g_FableConfigMemoryB_013750A0,
            g_registryConstructCalls,
            g_registryReadCalls,
            g_registryWriteCalls,
            g_registryDestroyCalls,
            g_observedWrite,
            g_dialogCalls,
            g_securityCalls,
            g_requirementsCalls);
    }
    const bool prompt = CheckPromptAcceptedFallback();
    const bool software = CheckDetectedSoftwareFallback();
    const bool passed =
        load && createMissing && createFailure && videoMissing &&
        videoFailure && normal && prompt && software;
    if (!passed)
    {
        printf(
            "FABLETLC_CONFIG_DETECTION_BEHAVIOR FAIL "
            "load=%u cm=%u cf=%u vm=%u vf=%u normal=%u prompt=%u sw=%u\n",
            load,
            createMissing,
            createFailure,
            videoMissing,
            videoFailure,
            normal,
            prompt,
            software);
        return 1;
    }
    printf("FABLETLC_CONFIG_DETECTION_BEHAVIOR PASS\n");
    return 0;
}
