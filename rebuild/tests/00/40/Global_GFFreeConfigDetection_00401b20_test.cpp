#include <stdio.h>
#include <string.h>

#include "fable_config_detection.h"

void* g_FableConfigDetectionModule_013B7C8C = 0;
FableGetProcAddressFunction g_FableGetProcAddress_0143FE44 = 0;
FableFreeLibraryFunction g_FableFreeLibrary_0143FE3C = 0;

namespace
{
    fable_u32 g_getProcCalls;
    fable_u32 g_releaseCalls;
    fable_u32 g_freeCalls;
    void* g_seenModule;
    bool g_exposeRelease;
    bool g_nameMatched;

    void FABLE_STDCALL Release()
    {
        ++g_releaseCalls;
    }

    void* FABLE_STDCALL GetProcAddressStub(void* module, const char* name)
    {
        ++g_getProcCalls;
        g_seenModule = module;
        g_nameMatched = strcmp(name, "Config_Release") == 0;
        return g_exposeRelease ? reinterpret_cast<void*>(&Release) : 0;
    }

    int FABLE_STDCALL FreeLibraryStub(void* module)
    {
        ++g_freeCalls;
        g_seenModule = module;
        return 1;
    }

    bool RunScenario(bool exposeRelease)
    {
        g_getProcCalls = 0;
        g_releaseCalls = 0;
        g_freeCalls = 0;
        g_seenModule = 0;
        g_exposeRelease = exposeRelease;
        g_nameMatched = false;
        g_FableConfigDetectionModule_013B7C8C =
            reinterpret_cast<void*>(0x12345678);

        const bool result = GFFreeConfigDetection();
        return
            !result &&
            g_getProcCalls == 1 &&
            g_releaseCalls == (exposeRelease ? 1UL : 0UL) &&
            g_freeCalls == 1 &&
            g_seenModule == reinterpret_cast<void*>(0x12345678) &&
            g_nameMatched &&
            g_FableConfigDetectionModule_013B7C8C == 0;
    }
}

int main()
{
    g_FableGetProcAddress_0143FE44 = &GetProcAddressStub;
    g_FableFreeLibrary_0143FE3C = &FreeLibraryStub;

    if (!RunScenario(false) || !RunScenario(true))
    {
        puts("FABLETLC_FREE_CONFIG_DETECTION_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_FREE_CONFIG_DETECTION_BEHAVIOR PASS");
    return 0;
}
