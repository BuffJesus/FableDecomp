#include "fable_config_detection.h"
#include "fable_eula.h"
#include "fable_gfmain.h"
#include "fable_string.h"
#include "fable_registry.h"
#include "engine/CConfigOptions.h"

#include <stdlib.h>
#include <string.h>

// kernel32, hand-declared (the integration units link no windows.h).
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(
    void* address,
    unsigned long size,
    unsigned long protect,
    unsigned long* oldProtect);

extern "C" char __ImageBase;

void* g_FableConfigDetectionModule_013B7C8C = 0;
FableGetProcAddressFunction g_FableGetProcAddress_0143FE44 = 0;
FableFreeLibraryFunction g_FableFreeLibrary_0143FE3C = 0;
FableLoadLibraryAFunction g_FableLoadLibraryA_0143FE40 = 0;
FableMessageBoxAFunction g_FableMessageBoxA_014402C4 = 0;

// Runtime stand-ins for the configuration globals and registry. No host registry writes.
ConfigDetection* pConfig = 0;
CConfigOptions GConfigOptions = {
    -1, -1, -1, -1, -1, 640, 30, -1, -1, -1, -1, 1, 2
};
const CWideString CSystemRegistry::GAME_REG_KEY;
const CWideString CSystemRegistry::GFX_RESET_KEY_NAME;
// Both retail leaves name the same object at 0x013BCA28.
#pragma comment(linker, "/alternatename:_g_FableEulaLanguage_013BCA28=?GAME_REG_KEY@CSystemRegistry@@2VCWideString@@B")

// CWideString::ToCharString @ 0x0099B2C0: narrows each code unit into a
// scratch buffer (retail uses a malloc'd one) and builds the CCharString.
CCharString CWideString::ToCharString() const
{
    static char* s_narrow = 0;
    static long s_capacity = 0;

    const wchar_t* text = *this;
    long length = 0;
    while (text[length] != L'\0')
        ++length;

    if (length + 1 > s_capacity)
    {
        char* grown = static_cast<char*>(realloc(s_narrow, length + 1));
        if (grown == 0)
            return CCharString("", 0);
        s_narrow = grown;
        s_capacity = length + 1;
    }
    for (long index = 0; index < length; ++index)
        s_narrow[index] = static_cast<char>(text[index]);
    s_narrow[length] = '\0';
    return CCharString(s_narrow, length);
}
extern "C" const char* g_FableEulaDocument_01374F54 = "eula.rtf";
extern "C" const char* g_FableWarrantyDocument_01374F58 =
    "warranty.rtf";

namespace
{
    fable_u8 g_ConfigInfo[0xC0];
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

    // EULA::DoEULAThings (0x00401FE0) calls LoadLibraryW/GetProcAddress/
    // FreeLibrary through the import table. While it runs the boundary points
    // this image's slots at these recorders: eula.dll "loads" as a token
    // module whose EBUEula accepts; every other call goes to kernel32.
    typedef void* (FABLE_STDCALL* LoadLibraryWFunction)(const wchar_t*);
    typedef void* (FABLE_STDCALL* GetProcAddressFunction)(
        void*,
        const char*);
    typedef int (FABLE_STDCALL* FreeLibraryFunction)(void*);

    LoadLibraryWFunction g_RealLoadLibraryW;
    GetProcAddressFunction g_RealGetProcAddress;
    FreeLibraryFunction g_RealFreeLibrary;

    void* const kEulaModule = reinterpret_cast<void*>(0xE11A0001);

    void* FABLE_STDCALL EulaLoadLibrary(const wchar_t* name)
    {
        static const wchar_t expected[] = L"eula.dll";
        if (name == 0 || memcmp(name, expected, sizeof(expected)) != 0)
            return g_RealLoadLibraryW(name);
        return kEulaModule;
    }

    void* FABLE_STDCALL EulaGetProcAddress(void* module, const char* name)
    {
        if (module != kEulaModule)
            return g_RealGetProcAddress(module, name);
        return
            name != 0 && strcmp(name, "EBUEula") == 0
                ? reinterpret_cast<void*>(&EulaExport)
                : 0;
    }

    int FABLE_STDCALL EulaFreeLibrary(void* module)
    {
        if (module != kEulaModule)
            return g_RealFreeLibrary(module);
        if (g_ActivePhase8State != 0)
            ++g_ActivePhase8State->exactEulaFreeCalls;
        return 1;
    }

    void** FindImportSlot(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        const fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        if (rva == 0)
            return 0;
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                {
                    return &slots[i];
                }
            }
        }
        return 0;
    }

    bool SwapImport(const char* name, void* replacement, void** previous)
    {
        void** slot = FindImportSlot(name);
        unsigned long oldProtect = 0;
        if (slot == 0 ||
            !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
        {
            return false;
        }
        if (previous != 0)
            *previous = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
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

extern "C" void FABLE_FASTCALL InitExternalStrings() {}
extern "C" int FABLE_STDCALL DisplayErrorDialog(int*, int) { return 0; }
CSystemRegistry::CSystemRegistry(const CWideString&, long) : rootKey_(0), openKey_(0) {}
CSystemRegistry::~CSystemRegistry() {}
int CSystemRegistry::GetBOOL(const CWideString&, int) const { return 0; }
void CSystemRegistry::WriteBOOL(const CWideString&, int) const {}
// user32 is intentionally absent from this headless runtime.
extern "C" FableMessageBoxAFunction FableConfigMessageBoxImport = &ConfigMessageBox;
#pragma comment(linker, "/alternatename:__imp__MessageBoxA@16=_FableConfigMessageBoxImport")

bool FABLE_FASTCALL FableRunExactPhase8EulaBoundary(
    GFMainPhase8State& state)
{
    const bool redirected =
        SwapImport(
            "LoadLibraryW",
            reinterpret_cast<void*>(&EulaLoadLibrary),
            reinterpret_cast<void**>(&g_RealLoadLibraryW)) &&
        SwapImport(
            "GetProcAddress",
            reinterpret_cast<void*>(&EulaGetProcAddress),
            reinterpret_cast<void**>(&g_RealGetProcAddress)) &&
        SwapImport(
            "FreeLibrary",
            reinterpret_cast<void*>(&EulaFreeLibrary),
            reinterpret_cast<void**>(&g_RealFreeLibrary));

    // Without the redirection the EULA would look for a real eula.dll.
    bool result = false;
    if (redirected)
    {
        g_ActivePhase8State = &state;
        result = EULA::DoEULAThings() != 0;
        g_ActivePhase8State = 0;
    }

    if (g_RealLoadLibraryW != 0)
        SwapImport("LoadLibraryW", reinterpret_cast<void*>(g_RealLoadLibraryW), 0);
    if (g_RealGetProcAddress != 0)
        SwapImport(
            "GetProcAddress",
            reinterpret_cast<void*>(g_RealGetProcAddress),
            0);
    if (g_RealFreeLibrary != 0)
        SwapImport("FreeLibrary", reinterpret_cast<void*>(g_RealFreeLibrary), 0);
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
    void* load = 0;
    void* proc = 0;
    const bool redirected =
        SwapImport("LoadLibraryA", reinterpret_cast<void*>(&ConfigLoadLibrary), &load) &&
        SwapImport("GetProcAddress", reinterpret_cast<void*>(&ConfigGetProcAddress), &proc);
    g_ActivePhase8State = &state;
    const bool result = redirected && GFConfigDetection();
    g_ActivePhase8State = 0;
    if (proc) SwapImport("GetProcAddress", proc, 0);
    if (load) SwapImport("LoadLibraryA", load, 0);
    return result;
}

void FABLE_FASTCALL FableRunExactPhase8FreeConfigurationBoundary(
    GFMainPhase8State& state)
{
    g_ActivePhase8State = &state;
    GFFreeConfigDetection();
    g_ActivePhase8State = 0;
}
