#include "fable_gfmain.h"
#include "fable_window_title.h"

#include <stdlib.h>
#include <string.h>

void FABLE_FASTCALL FableConstructWideString(CWideString* value);

extern "C" fable_u8 g_FableEmptyCharString_0129AAF4 = 0;
extern "C" fable_u8 g_FableWindowTitleValue_0122D820 = 0;

namespace
{
    const wchar_t kBoundaryWindowTitle[] =
        L"Fable - The Lost Chapters";
    const char kBoundaryWindowTitleTag[] =
        "TEXT_GUI_WINDOW_TITLE";
    const char kBoundaryFontBankName[] = "TEXT_ENGLISH_MAIN";

    CCharStringData g_WindowTitleTagData = {
        const_cast<char*>(kBoundaryWindowTitleTag),
        sizeof(kBoundaryWindowTitleTag) - 1,
        0,
        0,
        1
    };
    CCharStringData g_FontBankNameData = {
        const_cast<char*>(kBoundaryFontBankName),
        sizeof(kBoundaryFontBankName) - 1,
        0,
        0,
        1
    };

    struct FableWindowTitleBank
    {
        void** vtable;
    };

    void* g_WindowTitleBankVTable[2];
    GFMainPhase7State* g_ActivePhase7State;

    void __fastcall DeleteWindowTitleBank(
        void* bank,
        void*,
        fable_u32 flags)
    {
        if (g_ActivePhase7State != 0)
            ++g_ActivePhase7State->exactWindowTitleBankDeleteCalls;
        if ((flags & 1) != 0)
            free(bank);
    }

    void __fastcall OpenWindowTitleBank(
        void*,
        void*,
        const CCharString*,
        fable_u32)
    {
        if (g_ActivePhase7State != 0)
            ++g_ActivePhase7State->exactWindowTitleBankOpenCalls;
    }
}

extern "C" void __fastcall
FableWindowTitleWideLiteralCtor_004023f0(
    void* destination,
    void*,
    const wchar_t*)
{
    *static_cast<void**>(destination) = 0;
}

extern "C" void __fastcall
FableWindowTitleCharLiteralCtor_004023f0(
    void* destination,
    void*,
    const char*,
    long)
{
    *static_cast<CCharStringData**>(destination) =
        &g_WindowTitleTagData;
}

extern "C" void* __cdecl
FableWindowTitleOperatorNew_004023f0(fable_u32 size)
{
    return malloc(size);
}

extern "C" void* __fastcall
FableWindowTitleBankCtor_004023f0(void* destination, void*)
{
    g_WindowTitleBankVTable[0] =
        reinterpret_cast<void*>(&DeleteWindowTitleBank);
    g_WindowTitleBankVTable[1] =
        reinterpret_cast<void*>(&OpenWindowTitleBank);
    static_cast<FableWindowTitleBank*>(destination)->vtable =
        g_WindowTitleBankVTable;
    return destination;
}

extern "C" void* __fastcall
FableWindowTitleGetFontBankName_004023f0(
    void* destination,
    void*)
{
    *static_cast<CCharStringData**>(destination) =
        &g_FontBankNameData;
    return destination;
}

extern "C" void __fastcall
FableWindowTitleCharDtor_004023f0(void*, void*)
{
}

extern "C" fable_u32 __fastcall
FableWindowTitleCrc_004023f0(
    fable_u32,
    const void*,
    fable_u32)
{
    return 0x12345678;
}

extern "C" fable_u32 __fastcall
FableWindowTitleLookup_004023f0(void*, void*, fable_u32)
{
    if (g_ActivePhase7State != 0)
        ++g_ActivePhase7State->exactWindowTitleLookupCalls;
    return 0;
}

extern "C" void __fastcall
FableWindowTitleGetGroupEntry_004023f0(
    void*,
    void*,
    void* destination,
    fable_u32,
    bool)
{
    *static_cast<void**>(destination) = 0;
}

extern "C" void* __fastcall
FableWindowTitleGetBankHandle_004023f0(
    void*,
    void* output,
    void*)
{
    *static_cast<void**>(output) = 0;
    return output;
}

extern "C" void* __fastcall
FableWindowTitleWideAssign_004023f0(
    void* destination,
    void*,
    const void*)
{
    return destination;
}

extern "C" void __fastcall
FableWindowTitleWideDtor_004023f0(void*, void*)
{
}

extern "C" void* __fastcall
FableWindowTitleAssignResult_004023f0(
    void* destination,
    const void*,
    const void*)
{
    CWideString* result = static_cast<CWideString*>(destination);
    FableConstructWideString(result);
    *result = kBoundaryWindowTitle;
    return destination;
}

CWideString FABLE_FASTCALL FableRunExactWindowTitleBoundary(
    GFMainPhase7State& state)
{
    ++state.exactWindowTitleCoordinatorCalls;
    g_ActivePhase7State = &state;
    CWideString title = GetWindowTitle();
    g_ActivePhase7State = 0;
    return title;
}
