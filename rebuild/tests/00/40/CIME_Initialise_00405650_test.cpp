#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_ime.h"

extern "C" void* g_FableImeDrawRectCallback_0143CD64 = 0;
extern "C" void* g_FableImeDrawFansCallback_0143E514 = 0;
extern "C" void* g_FableImeOnCharCallback_0143CF08 = 0;
extern "C" void* g_FableImeRenderCallback_0143CDFC = 0;
extern "C" void* g_FableImeGetCaretCallback_0143E630 = 0;
extern "C" fable_u8 g_FableImeFontVTable_0122DCBC = 0;
extern "C" void FableImeDrawRectCallback_00404D50();
extern "C" void FableImeDrawFansCallback_00404A60();
extern "C" void FableImeOnCharCallback_00404A70();
extern "C" void FableImeRenderCallback_00404E10();
extern "C" void FableImeGetCaretCallback_00404F00();

namespace
{
    struct FakeSystemManager
    {
        fable_u8 padding00[0x60];
        void* drawContext;
    };

    FakeSystemManager g_systemManager;
    bool g_initialiseResult;
    fable_u32 g_cleanupCalls;
    fable_u32 g_initialiseCalls;
    fable_u8 g_initialiseMode;
    void* g_initialiseWindow;
    fable_u32 g_allocations;
    fable_u32 g_appearanceCalls;
    void* g_appearance;
    fable_u32 g_contextCalls;
    fable_u32 g_drawCalls;
    void* g_drawFirst;
    void* g_drawSecond;
    fable_u32 g_resetCalls;
    fable_u8 g_resetValue;
    fable_u32 g_enableCalls;
    fable_u8 g_enableValue;

    void Reset()
    {
        g_initialiseResult = false;
        g_cleanupCalls = 0;
        g_initialiseCalls = 0;
        g_initialiseMode = 0xFF;
        g_initialiseWindow = 0;
        g_allocations = 0;
        g_appearanceCalls = 0;
        g_appearance = 0;
        g_contextCalls = 0;
        g_drawCalls = 0;
        g_drawFirst = 0;
        g_drawSecond = 0;
        g_resetCalls = 0;
        g_resetValue = 0xFF;
        g_enableCalls = 0;
        g_enableValue = 0xFF;
        g_systemManager.drawContext = reinterpret_cast<void*>(0x12345678);
    }

    NUISystem::CIME& FreshIme(unsigned char (&storage)[sizeof(NUISystem::CIME)])
    {
        memset(storage, 0, sizeof(storage));
        return *reinterpret_cast<NUISystem::CIME*>(storage);
    }

    bool CheckDisabled()
    {
        Reset();
        unsigned char storage[sizeof(NUISystem::CIME)];
        NUISystem::CIME& ime = FreshIme(storage);
        HWND__* window = reinterpret_cast<HWND__*>(0x11112222);
        ime.Initialise(window);
        return
            g_cleanupCalls == 1 &&
            g_initialiseCalls == 1 &&
            g_initialiseWindow == window &&
            g_initialiseMode == 1 &&
            g_allocations == 0 &&
            g_resetCalls == 0 &&
            g_enableCalls == 0;
    }

    bool CheckEnabledFailure()
    {
        Reset();
        unsigned char storage[sizeof(NUISystem::CIME)];
        storage[0] = 1;
        NUISystem::CIME& ime = *reinterpret_cast<NUISystem::CIME*>(storage);
        HWND__* window = reinterpret_cast<HWND__*>(0x33334444);
        ime.Initialise(window);
        return
            g_initialiseMode == 0 &&
            *reinterpret_cast<HWND__**>(storage + 4) == window &&
            storage[8] == 0 &&
            g_FableImeDrawRectCallback_0143CD64 ==
                reinterpret_cast<void*>(&FableImeDrawRectCallback_00404D50) &&
            g_FableImeGetCaretCallback_0143E630 ==
                reinterpret_cast<void*>(&FableImeGetCaretCallback_00404F00) &&
            g_allocations == 0 &&
            g_drawCalls == 0;
    }

    bool CheckEnabledSuccess()
    {
        Reset();
        g_initialiseResult = true;
        unsigned char storage[sizeof(NUISystem::CIME)];
        storage[0] = 1;
        NUISystem::CIME& ime = *reinterpret_cast<NUISystem::CIME*>(storage);
        ime.Initialise(reinterpret_cast<HWND__*>(0x55556666));
        void* font = *reinterpret_cast<void**>(storage + 0x1C);
        const bool passed =
            storage[8] == 1 &&
            g_allocations == 1 &&
            font != 0 &&
            *reinterpret_cast<void**>(font) ==
                &g_FableImeFontVTable_0122DCBC &&
            g_appearanceCalls == 1 &&
            g_appearance == storage + 0x0C &&
            g_contextCalls == 1 &&
            g_drawCalls == 1 &&
            g_drawFirst == reinterpret_cast<void*>(0xAAAABBBB) &&
            g_drawSecond == reinterpret_cast<void*>(0xCCCCDDDD) &&
            g_resetCalls == 1 &&
            g_resetValue == 0 &&
            g_enableCalls == 1 &&
            g_enableValue == 0;
        ::operator delete(font);
        return passed;
    }
}

extern "C" void FableImeDrawRectCallback_00404D50() {}
extern "C" void FableImeDrawFansCallback_00404A60() {}
extern "C" void FableImeOnCharCallback_00404A70() {}
extern "C" void FableImeRenderCallback_00404E10() {}
extern "C" void FableImeGetCaretCallback_00404F00() {}

extern "C" void __fastcall FableImeCleanup_00405650(void*, void*)
{
    ++g_cleanupCalls;
}

extern "C" bool __fastcall
FableImeUiInitialise_00405650(void* window, fable_u8 mode)
{
    ++g_initialiseCalls;
    g_initialiseWindow = window;
    g_initialiseMode = mode;
    return g_initialiseResult;
}

extern "C" void* __cdecl FableImeOperatorNew_00405650(fable_u32 size)
{
    ++g_allocations;
    return ::operator new(size);
}

extern "C" void __fastcall
FableImeSetAppearance_00405650(void* appearance, void*)
{
    ++g_appearanceCalls;
    g_appearance = appearance;
}

extern "C" void* FableImeGetSystemManager_00405650()
{
    return &g_systemManager;
}

extern "C" void __fastcall
FableImeGetDrawContext_00405650(void*, void*, void* output)
{
    ++g_contextCalls;
    void** values = reinterpret_cast<void**>(output);
    values[0] = reinterpret_cast<void*>(0xAAAABBBB);
    values[1] = reinterpret_cast<void*>(0xCCCCDDDD);
}

extern "C" void __fastcall FableImeDraw_00405650(void* first, void* second)
{
    ++g_drawCalls;
    g_drawFirst = first;
    g_drawSecond = second;
}

extern "C" void __fastcall
FableImeResetComposition_00405650(fable_u8 value, void*)
{
    ++g_resetCalls;
    g_resetValue = value;
}

extern "C" void __fastcall FableImeEnable_00405650(fable_u8 value, void*)
{
    ++g_enableCalls;
    g_enableValue = value;
}

int main()
{
    if (!CheckDisabled() || !CheckEnabledFailure() || !CheckEnabledSuccess())
    {
        puts("FABLETLC_CIME_INITIALISE_BEHAVIOR FAIL");
        return 1;
    }
    puts("FABLETLC_CIME_INITIALISE_BEHAVIOR PASS");
    return 0;
}
