#include <stdio.h>
#include <string.h>

#include "fable_eula.h"

extern "C" fable_u8 g_FableEulaLanguage_013BCA28 = 0;
extern "C" fable_u8 g_FableEulaEmptyText_0129AAF4 = 0;
extern "C" const char* g_FableEulaDocument_01374F54 = "eula.rtf";
extern "C" const char* g_FableWarrantyDocument_01374F58 = "warranty.rtf";

namespace
{
    struct FakeStringData
    {
        const char* text;
    };

    bool g_loadSucceeds;
    bool g_exportPresent;
    int g_exportResult;
    const char* g_language;
    fable_u32 g_loadCalls;
    fable_u32 g_getProcCalls;
    fable_u32 g_convertCalls;
    fable_u32 g_exportCalls;
    fable_u32 g_destroyCalls;
    fable_u32 g_freeCalls;
    const char* g_observedLanguage;
    const char* g_observedEula;
    const char* g_observedWarranty;
    int g_observedMode;
    FakeStringData g_stringData;

    void Reset()
    {
        g_loadSucceeds = true;
        g_exportPresent = true;
        g_exportResult = 1;
        g_language = "English";
        g_loadCalls = 0;
        g_getProcCalls = 0;
        g_convertCalls = 0;
        g_exportCalls = 0;
        g_destroyCalls = 0;
        g_freeCalls = 0;
        g_observedLanguage = 0;
        g_observedEula = 0;
        g_observedWarranty = 0;
        g_observedMode = 0;
        g_stringData.text = g_language;
    }

    int FABLE_STDCALL TestExport(
        const char* language,
        const char* eula,
        const char* warranty,
        int mode)
    {
        ++g_exportCalls;
        g_observedLanguage = language;
        g_observedEula = eula;
        g_observedWarranty = warranty;
        g_observedMode = mode;
        return g_exportResult;
    }

    bool CheckLoadFailure()
    {
        Reset();
        g_loadSucceeds = false;
        return EULA::DoEULAThings() == 0 &&
            g_loadCalls == 1 &&
            g_getProcCalls == 0 &&
            g_convertCalls == 0 &&
            g_freeCalls == 0;
    }

    bool CheckMissingExport()
    {
        Reset();
        g_exportPresent = false;
        return EULA::DoEULAThings() == 0 &&
            g_getProcCalls == 1 &&
            g_convertCalls == 0 &&
            g_freeCalls == 0;
    }

    bool CheckRejected()
    {
        Reset();
        g_exportResult = 0;
        return EULA::DoEULAThings() == 0 &&
            g_convertCalls == 1 &&
            g_exportCalls == 1 &&
            g_destroyCalls == 1 &&
            g_freeCalls == 0;
    }

    bool CheckAcceptedLanguage()
    {
        Reset();
        const int result = EULA::DoEULAThings();
        return result == 1 &&
            g_observedLanguage == g_language &&
            g_observedEula == g_FableEulaDocument_01374F54 &&
            g_observedWarranty == g_FableWarrantyDocument_01374F58 &&
            g_observedMode == 1 &&
            g_destroyCalls == 1 &&
            g_freeCalls == 1;
    }

    bool CheckAcceptedFallback()
    {
        Reset();
        g_language = 0;
        g_stringData.text = 0;
        const int result = EULA::DoEULAThings();
        return result == 1 &&
            g_observedLanguage ==
                reinterpret_cast<const char*>(
                    &g_FableEulaEmptyText_0129AAF4) &&
            g_destroyCalls == 1 &&
            g_freeCalls == 1;
    }
}

extern "C" void* FABLE_STDCALL
TestLoadLibraryW(const unsigned short* name)
{
    ++g_loadCalls;
    static const unsigned short expected[] = L"eula.dll";
    return
        g_loadSucceeds && name &&
            memcmp(name, expected, sizeof(expected)) == 0
            ? reinterpret_cast<void*>(0x11112222)
            : 0;
}

extern "C" void* FABLE_STDCALL
TestGetProcAddress(void*, const char* name)
{
    ++g_getProcCalls;
    return
        g_exportPresent && name && strcmp(name, "EBUEula") == 0
            ? reinterpret_cast<void*>(&TestExport)
            : 0;
}

extern "C" int FABLE_STDCALL TestFreeLibrary(void*)
{
    ++g_freeCalls;
    return 1;
}

extern "C" void* (FABLE_STDCALL* g_FableEulaLoadLibraryW_00401fe0)(
    const unsigned short*) = &TestLoadLibraryW;
extern "C" void* (FABLE_STDCALL* g_FableEulaGetProcAddress_00401fe0)(
    void*,
    const char*) = &TestGetProcAddress;
extern "C" int (FABLE_STDCALL* g_FableEulaFreeLibrary_00401fe0)(void*) =
    &TestFreeLibrary;

extern "C" void* __fastcall FableEulaConvertLanguage_00401fe0(
    void*,
    void*,
    void* output)
{
    ++g_convertCalls;
    g_stringData.text = g_language;
    *reinterpret_cast<FakeStringData**>(output) =
        g_language ? &g_stringData : 0;
    return output;
}

extern "C" void __fastcall
FableEulaDestroyLanguage_00401fe0(void*, void*)
{
    ++g_destroyCalls;
}

int main()
{
    const bool load = CheckLoadFailure();
    const bool missing = CheckMissingExport();
    const bool rejected = CheckRejected();
    const bool accepted = CheckAcceptedLanguage();
    const bool fallback = CheckAcceptedFallback();
    const bool passed = load && missing && rejected && accepted && fallback;
    if (!passed)
    {
        printf(
            "FABLETLC_EULA_BEHAVIOR FAIL "
            "load=%u missing=%u rejected=%u accepted=%u fallback=%u\n",
            load,
            missing,
            rejected,
            accepted,
            fallback);
        return 1;
    }
    printf("FABLETLC_EULA_BEHAVIOR PASS\n");
    return 0;
}
