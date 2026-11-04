#include "fable_eula.h"
#include "fable_string.h"

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

// The game's registry key name (GFConfigDetection opens the same string);
// eula.dll gets it with the EULA and warranty documents.
extern "C" CWideString g_FableEulaLanguage_013BCA28;
extern "C" const char* g_FableEulaDocument_01374F54;
extern "C" const char* g_FableWarrantyDocument_01374F58;

typedef int (FABLE_STDCALL* EBUEulaFunction)(
    const char* registryKey,
    const char* eulaFile,
    const char* warrantyFile,
    int flags);

int FABLE_FASTCALL EULA::DoEULAThings()
{
    FableModuleHandle library = LoadLibraryW(L"eula.dll");
    if (library == 0)
        return 0;

    EBUEulaFunction eula = reinterpret_cast<EBUEulaFunction>(
        GetProcAddress(library, "EBUEula"));
    if (eula != 0)
    {
        const bool accepted = eula(
            g_FableEulaLanguage_013BCA28.ToCharString().GetConstCharPtr(),
            g_FableEulaDocument_01374F54,
            g_FableWarrantyDocument_01374F58,
            1) == 1;
        if (accepted)
        {
            FreeLibrary(library);
            return 1;
        }
    }
    return 0;
}
