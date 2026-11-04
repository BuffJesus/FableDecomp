#include <stdio.h>
#include <string.h>

#include "fable_eula.h"
#include "fable_string.h"

// DoEULAThings reaches kernel32 through the import table, so the fixture
// redirects its own LoadLibraryW/GetProcAddress/FreeLibrary slots to recorders
// and forwards every call that is not the EULA's to the real function.

struct HINSTANCE__;
typedef HINSTANCE__* FableModuleHandle;
typedef int (FABLE_STDCALL* FableFarProc)();

extern "C" __declspec(dllimport) FableModuleHandle FABLE_STDCALL LoadLibraryW(
    const wchar_t* fileName);
extern "C" __declspec(dllimport) FableFarProc FABLE_STDCALL GetProcAddress(
    FableModuleHandle module,
    const char* procName);
extern "C" __declspec(dllimport) int FABLE_STDCALL FreeLibrary(
    FableModuleHandle module);
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(
    void* address,
    unsigned long size,
    unsigned long protect,
    unsigned long* oldProtect);

extern "C" char __ImageBase;

// The registry key CWideString; the fixture only needs its address.
extern "C" fable_u32 g_FableEulaLanguage_013BCA28 = 0x4B455952;
extern "C" const char* g_FableEulaDocument_01374F54 = "eula.rtf";
extern "C" const char* g_FableWarrantyDocument_01374F58 = "warranty.rtf";
char g_FableEmptyNarrowString_0129AAF4[1] = "";

namespace
{
    typedef FableModuleHandle (FABLE_STDCALL* LoadLibraryWFunction)(
        const wchar_t*);
    typedef FableFarProc (FABLE_STDCALL* GetProcAddressFunction)(
        FableModuleHandle,
        const char*);
    typedef int (FABLE_STDCALL* FreeLibraryFunction)(FableModuleHandle);

    LoadLibraryWFunction g_realLoadLibraryW;
    GetProcAddressFunction g_realGetProcAddress;
    FreeLibraryFunction g_realFreeLibrary;

    FableModuleHandle const kEulaModule =
        reinterpret_cast<FableModuleHandle>(0x11112222);

    CCharStringData g_stringData;
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
    fable_u32 g_destroyedAfterExport;
    const void* g_convertThis;
    const char* g_observedLanguage;
    const char* g_observedEula;
    const char* g_observedWarranty;
    int g_observedMode;

    void Reset()
    {
        g_loadSucceeds = true;
        g_exportPresent = true;
        g_exportResult = 1;
        g_language = "Software\\Lionhead Studios Ltd\\Fable";
        g_loadCalls = 0;
        g_getProcCalls = 0;
        g_convertCalls = 0;
        g_exportCalls = 0;
        g_destroyCalls = 0;
        g_freeCalls = 0;
        g_destroyedAfterExport = 0;
        g_convertThis = 0;
        g_observedLanguage = 0;
        g_observedEula = 0;
        g_observedWarranty = 0;
        g_observedMode = 0;
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

    FableModuleHandle FABLE_STDCALL TestLoadLibraryW(const wchar_t* name)
    {
        static const wchar_t expected[] = L"eula.dll";
        if (name == 0 || memcmp(name, expected, sizeof(expected)) != 0)
            return g_realLoadLibraryW(name);
        ++g_loadCalls;
        return g_loadSucceeds ? kEulaModule : 0;
    }

    FableFarProc FABLE_STDCALL TestGetProcAddress(
        FableModuleHandle module,
        const char* name)
    {
        if (module != kEulaModule)
            return g_realGetProcAddress(module, name);
        ++g_getProcCalls;
        return g_exportPresent && strcmp(name, "EBUEula") == 0
            ? reinterpret_cast<FableFarProc>(&TestExport)
            : 0;
    }

    int FABLE_STDCALL TestFreeLibrary(FableModuleHandle module)
    {
        if (module != kEulaModule)
            return g_realFreeLibrary(module);
        ++g_freeCalls;
        return 1;
    }

    void** FindImport(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
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
                    return &slots[i];
            }
        }
        return 0;
    }

    bool Patch(const char* name, void* replacement, void** original)
    {
        void** slot = FindImport(name);
        unsigned long oldProtect = 0;
        if (slot == 0 || !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
            return false;
        *original = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }

    bool InstallImports()
    {
        return
            Patch(
                "LoadLibraryW",
                reinterpret_cast<void*>(&TestLoadLibraryW),
                reinterpret_cast<void**>(&g_realLoadLibraryW)) &&
            Patch(
                "GetProcAddress",
                reinterpret_cast<void*>(&TestGetProcAddress),
                reinterpret_cast<void**>(&g_realGetProcAddress)) &&
            Patch(
                "FreeLibrary",
                reinterpret_cast<void*>(&TestFreeLibrary),
                reinterpret_cast<void**>(&g_realFreeLibrary));
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
            g_exportCalls == 0 &&
            g_freeCalls == 0;
    }

    bool CheckRejected()
    {
        Reset();
        g_exportResult = 2;
        return EULA::DoEULAThings() == 0 &&
            g_convertCalls == 1 &&
            g_exportCalls == 1 &&
            g_destroyCalls == 1 &&
            g_freeCalls == 0;
    }

    bool CheckAccepted()
    {
        Reset();
        const int result = EULA::DoEULAThings();
        return result == 1 &&
            g_convertThis == &g_FableEulaLanguage_013BCA28 &&
            g_observedLanguage == g_language &&
            g_observedEula == g_FableEulaDocument_01374F54 &&
            g_observedWarranty == g_FableWarrantyDocument_01374F58 &&
            g_observedMode == 1 &&
            g_destroyCalls == 1 &&
            g_destroyedAfterExport == 1 &&
            g_freeCalls == 1;
    }

    bool CheckAcceptedEmptyKey()
    {
        Reset();
        g_language = 0;
        const int result = EULA::DoEULAThings();
        return result == 1 &&
            g_observedLanguage == g_FableEmptyNarrowString_0129AAF4 &&
            g_destroyCalls == 1 &&
            g_freeCalls == 1;
    }
}

CCharString::CCharString(const char* text, long)
{
    g_stringData.text = const_cast<char*>(text);
    storage_ = text ? &g_stringData : 0;
}

CCharString::~CCharString()
{
    ++g_destroyCalls;
    if (g_exportCalls != 0)
        ++g_destroyedAfterExport;
}

CCharString CWideString::ToCharString() const
{
    ++g_convertCalls;
    g_convertThis = this;
    return CCharString(g_language, 0);
}

int main()
{
    if (!InstallImports())
    {
        printf("FABLETLC_EULA_BEHAVIOR FAIL imports\n");
        return 1;
    }
    const bool load = CheckLoadFailure();
    const bool missing = CheckMissingExport();
    const bool rejected = CheckRejected();
    const bool accepted = CheckAccepted();
    const bool empty = CheckAcceptedEmptyKey();
    const bool passed = load && missing && rejected && accepted && empty;
    if (!passed)
    {
        printf(
            "FABLETLC_EULA_BEHAVIOR FAIL "
            "load=%u missing=%u rejected=%u accepted=%u empty=%u\n",
            load,
            missing,
            rejected,
            accepted,
            empty);
        return 1;
    }
    printf("FABLETLC_EULA_BEHAVIOR PASS\n");
    return 0;
}
