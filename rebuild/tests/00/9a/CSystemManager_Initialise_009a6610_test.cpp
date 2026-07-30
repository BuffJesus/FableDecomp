#include <stdio.h>
#include <string.h>

#include "fable_gfuninitialise.h"

extern "C" fable_u8 g_FableDriveManagerVTable_0129AFF4;
extern "C" void* g_FableDriveManagerPointer_013CAA28;
extern "C" fable_u8 g_FableSystemInitialiseCount_013CA608;

namespace
{
    struct Calls
    {
        fable_u32 wideAssign;
        fable_u32 preInitialise;
        fable_u32 convertFiber;
        fable_u32 coInitialise;
        fable_u32 criticalSection;
        fable_u32 guid;
        fable_u32 initWindow;
        fable_u32 uninitialise;
        fable_u32 currentProcess;
        fable_u32 scratch;
        fable_u32 allocations;
        fable_u32 displayCopy;
        fable_u32 displayConstruct;
        fable_u32 displayReset;
        fable_u32 displayInitialise;
        fable_u32 stringDestroy;
        fable_u32 inputConstruct;
        fable_u32 inputReset;
        fable_u32 driveReset;
        fable_u32 driveInitialise;
        fable_u32 profileConstruct;
        fable_u32 profileReset;
        fable_u32 fontConstruct;
        fable_u32 fontReset;
        fable_u32 textConstruct;
        fable_u32 textReset;
        fable_u32 meshConstruct;
        fable_u32 meshReset;
        fable_u32 addComponent;
        fable_u32 coloursConstruct;
        fable_u32 coloursReset;
        fable_u32 cosine;
        fable_u32 vectorMath;
        fable_u32 initColours;
        fable_u32 initApplicationName;
    };

    Calls g_calls;
    bool g_preInitialiseResult;
    bool g_windowResult;
    ESystemCode g_driveResult;
    unsigned char g_allocations[8][0x2C8];
    unsigned char g_managerStorage[0x180];
    unsigned char g_initStorage[0xC8];
    void* g_displayObject;
    void* g_inputObject;
    void* g_profileObject;
    void* g_fontObject;
    void* g_textObject;
    void* g_meshObject;
    void* g_coloursObject;
    void* g_observedWindowFirst;
    fable_u32 g_observedWindowSecond;
    const void* g_observedDisplayInit;
    const void* g_observedInputInit;
    const void* g_observedProfileInit;
    const void* g_observedFontA;
    const void* g_observedFontB;
    fable_u32 g_observedExclusive;
    fable_u32 g_observedScratchSize;

    fable_u32& Dword(unsigned char* value, size_t offset)
    {
        return *reinterpret_cast<fable_u32*>(value + offset);
    }

    void Reset()
    {
        memset(&g_calls, 0, sizeof(g_calls));
        memset(g_allocations, 0, sizeof(g_allocations));
        memset(g_managerStorage, 0xCD, sizeof(g_managerStorage));
        g_managerStorage[0x09] = 0;
        memset(g_initStorage, 0, sizeof(g_initStorage));
        g_preInitialiseResult = true;
        g_windowResult = true;
        g_driveResult = kSystemCodeSuccess;
        g_displayObject = 0;
        g_inputObject = 0;
        g_profileObject = 0;
        g_fontObject = 0;
        g_textObject = 0;
        g_meshObject = 0;
        g_coloursObject = 0;
        g_observedWindowFirst = 0;
        g_observedWindowSecond = 0;
        g_observedDisplayInit = 0;
        g_observedInputInit = 0;
        g_observedProfileInit = 0;
        g_observedFontA = 0;
        g_observedFontB = 0;
        g_observedExclusive = 0;
        g_observedScratchSize = 0;
        g_FableDriveManagerPointer_013CAA28 = 0;
        g_FableSystemInitialiseCount_013CA608 = 0;

        Dword(g_initStorage, 0x00) = 0x11111111;
        Dword(g_initStorage, 0x04) = 0x22222222;
        Dword(g_initStorage, 0x08) = 7;
        Dword(g_initStorage, 0x24) = 0x33333333;
        Dword(g_initStorage, 0x28) = 0x44444444;
        Dword(g_initStorage, 0x9C) = 0;
        Dword(g_initStorage, 0xC0) = 0x55555555;
        Dword(g_initStorage, 0xC4) = 0x66666666;
        g_initStorage[0x10] = 1;
        g_initStorage[0x11] = 1;
        g_initStorage[0x20] = 1;
        g_initStorage[0x21] = 1;
        g_initStorage[0x34] = 1;
        g_initStorage[0x35] = 1;
    }

    CSystemManager& Manager()
    {
        return *reinterpret_cast<CSystemManager*>(g_managerStorage);
    }

    CSystemManagerInit* Init()
    {
        return reinterpret_cast<CSystemManagerInit*>(g_initStorage);
    }

    bool CheckEarlyBootstrapSkip()
    {
        Reset();
        g_preInitialiseResult = false;

        const ESystemCode result = Manager().Initialise(Init());

        return
            result == kSystemCodeSuccess &&
            g_calls.preInitialise == 1 &&
            g_calls.convertFiber == 0 &&
            g_calls.driveInitialise == 0 &&
            g_calls.coloursConstruct == 1 &&
            g_calls.coloursReset == 1 &&
            g_calls.cosine == 1 &&
            g_calls.vectorMath == 1 &&
            g_calls.initColours == 1 &&
            g_calls.initApplicationName == 1 &&
            g_FableSystemInitialiseCount_013CA608 == 1 &&
            g_managerStorage[0x09] == 1 &&
            g_managerStorage[0xDC] == 1 &&
            g_managerStorage[0xEA] == 1 &&
            Dword(g_managerStorage, 0x178) == 0x55555555 &&
            Dword(g_managerStorage, 0x17C) == 0x66666666;
    }

    bool CheckWindowFailure()
    {
        Reset();
        Dword(g_initStorage, 0x14) = 4;
        Dword(g_initStorage, 0x5C) = 0x77777777;
        Dword(g_initStorage, 0x60) = 0x88888888;
        g_windowResult = false;

        const ESystemCode result = Manager().Initialise(Init());

        return
            result == kSystemCodeWindowInitialisationFailed &&
            g_calls.convertFiber == 1 &&
            g_calls.coInitialise == 1 &&
            g_calls.criticalSection == 1 &&
            g_calls.guid == 1 &&
            g_calls.initWindow == 1 &&
            g_observedWindowFirst ==
                reinterpret_cast<void*>(0x77777777) &&
            g_observedWindowSecond == 0x88888888 &&
            g_calls.uninitialise == 1 &&
            g_calls.driveInitialise == 0 &&
            g_calls.coloursConstruct == 0;
    }

    bool CheckDriveFailurePropagation()
    {
        Reset();
        g_driveResult = static_cast<ESystemCode>(7);

        const ESystemCode result = Manager().Initialise(Init());

        return
            result == static_cast<ESystemCode>(7) &&
            g_calls.currentProcess == 1 &&
            g_calls.driveReset == 1 &&
            g_calls.driveInitialise == 1 &&
            g_FableDriveManagerPointer_013CAA28 != 0 &&
            *reinterpret_cast<void**>(
                g_FableDriveManagerPointer_013CAA28) ==
                &g_FableDriveManagerVTable_0129AFF4 &&
            g_calls.uninitialise == 1 &&
            g_calls.profileConstruct == 0 &&
            g_calls.coloursConstruct == 0;
    }

    bool CheckFullSuccess()
    {
        Reset();
        Dword(g_initStorage, 0x14) = 0x15;
        Dword(g_initStorage, 0x5C) = 0xABCDEF01;
        Dword(g_initStorage, 0x60) = 0x12345678;
        Dword(g_initStorage, 0xB4) = 0x4000;

        unsigned char fontA[8] = {0};
        unsigned char fontB[8] = {0};
        Dword(fontA, 4) = 1;
        Dword(fontB, 4) = 1;
        Dword(g_initStorage, 0x2C) =
            reinterpret_cast<fable_u32>(fontA);
        Dword(g_initStorage, 0x30) =
            reinterpret_cast<fable_u32>(fontB);

        const ESystemCode result = Manager().Initialise(Init());

        return
            result == kSystemCodeSuccess &&
            g_calls.initWindow == 1 &&
            g_managerStorage[0x8C] == 1 &&
            g_calls.scratch == 1 &&
            g_observedScratchSize == 0x4000 &&
            g_calls.displayCopy == 1 &&
            g_observedDisplayInit == g_initStorage + 0x5C &&
            g_calls.displayConstruct == 1 &&
            g_calls.displayReset == 1 &&
            g_calls.displayInitialise == 1 &&
            g_observedExclusive == 1 &&
            *reinterpret_cast<void**>(g_managerStorage + 0x60) ==
                g_displayObject &&
            g_calls.stringDestroy == 1 &&
            g_calls.inputConstruct == 1 &&
            g_observedInputInit == g_initStorage + 0x58 &&
            g_calls.inputReset == 1 &&
            *reinterpret_cast<void**>(g_managerStorage + 0x58) ==
                g_inputObject &&
            g_calls.driveInitialise == 1 &&
            g_calls.profileConstruct == 1 &&
            g_observedProfileInit == g_initStorage + 0x38 &&
            g_calls.profileReset == 1 &&
            *reinterpret_cast<void**>(g_managerStorage + 0x7C) ==
                g_profileObject &&
            g_calls.fontConstruct == 1 &&
            g_observedFontA == g_initStorage + 0x2C &&
            g_observedFontB == g_initStorage + 0x30 &&
            g_calls.fontReset == 1 &&
            *reinterpret_cast<void**>(g_managerStorage + 0x84) ==
                g_fontObject &&
            g_calls.textConstruct == 1 &&
            g_calls.textReset == 1 &&
            g_calls.meshConstruct == 1 &&
            g_calls.meshReset == 1 &&
            g_calls.addComponent == 1 &&
            g_calls.coloursConstruct == 1 &&
            g_calls.coloursReset == 1 &&
            g_calls.uninitialise == 0 &&
            g_calls.allocations == 8 &&
            g_managerStorage[0x09] == 1 &&
            g_managerStorage[0xDC] == 1;
    }
}

extern "C" fable_u8 g_FableDriveManagerVTable_0129AFF4 = 0;
extern "C" void* g_FableDriveManagerPointer_013CAA28 = 0;
extern "C" fable_u8 g_FableScratchBuffer_013CB498 = 0;
extern "C" fable_u8 g_FableSystemInitialiseCount_013CA608 = 0;

extern "C" void* FABLE_STDCALL TestConvertThreadToFiber(void*)
{
    ++g_calls.convertFiber;
    return reinterpret_cast<void*>(0xFA1B0001);
}

extern "C" long FABLE_STDCALL TestCoInitialize(void*)
{
    ++g_calls.coInitialise;
    return 0;
}

extern "C" void FABLE_STDCALL TestInitializeCriticalSection(void*)
{
    ++g_calls.criticalSection;
}

extern "C" void* FABLE_STDCALL TestGetCurrentProcess()
{
    ++g_calls.currentProcess;
    return reinterpret_cast<void*>(0xFFFFFFFF);
}

extern "C" void* (FABLE_STDCALL* g_FableConvertThreadToFiber_009a6610)(void*) =
    &TestConvertThreadToFiber;
extern "C" long (FABLE_STDCALL* g_FableCoInitialize_009a6610)(void*) =
    &TestCoInitialize;
extern "C" void (FABLE_STDCALL* g_FableInitializeCriticalSection_009a6610)(void*) =
    &TestInitializeCriticalSection;
extern "C" void* (FABLE_STDCALL* g_FableGetCurrentProcess_009a6610)() =
    &TestGetCurrentProcess;

extern "C" bool __fastcall
FableSystemPreInitialise_009a6610(void*, void*)
{
    ++g_calls.preInitialise;
    return g_preInitialiseResult;
}

extern "C" void __fastcall
FableSystemWideAssign_009a6610(void*, void*, const void*)
{
    ++g_calls.wideAssign;
}

extern "C" void __fastcall
FableSystemInitGuid_009a6610(void*, void*, const void*)
{
    ++g_calls.guid;
}

extern "C" void FableSystemDefaultErrorCallback_009a6610() {}

extern "C" bool __fastcall FableSystemInitWindow_009a6610(
    void*,
    void*,
    fable_u32 first,
    fable_u32 second)
{
    ++g_calls.initWindow;
    g_observedWindowFirst = reinterpret_cast<void*>(first);
    g_observedWindowSecond = second;
    return g_windowResult;
}

extern "C" void __fastcall
FableSystemUninitialise_009a6610(void*, void*)
{
    ++g_calls.uninitialise;
}

extern "C" void __fastcall FableSystemScratchInitialise_009a6610(
    void*,
    void*,
    fable_u32 size)
{
    ++g_calls.scratch;
    g_observedScratchSize = size;
}

extern "C" void __fastcall FableSystemDisplayInitCopy_009a6610(
    void*,
    void*,
    const void* source)
{
    ++g_calls.displayCopy;
    g_observedDisplayInit = source;
}

extern "C" void* __cdecl FableSystemOperatorNew_009a6610(fable_u32)
{
    void* result = g_allocations[g_calls.allocations];
    ++g_calls.allocations;
    return result;
}

extern "C" void* __fastcall FableSystemDisplayConstruct_009a6610(
    void* object,
    void*,
    void*)
{
    ++g_calls.displayConstruct;
    g_displayObject = object;
    return object;
}

extern "C" void __fastcall FableSystemDisplayReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.displayReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void __fastcall FableSystemDisplayInitialise_009a6610(
    void*,
    void*,
    void*,
    fable_u32 exclusive)
{
    ++g_calls.displayInitialise;
    g_observedExclusive = exclusive;
}

extern "C" void __fastcall
FableSystemCharStringDestroy_009a6610(void*, void*)
{
    ++g_calls.stringDestroy;
}

extern "C" void* __fastcall FableSystemInputConstruct_009a6610(
    void* object,
    void*,
    const void* init,
    fable_u32)
{
    ++g_calls.inputConstruct;
    g_inputObject = object;
    g_observedInputInit = init;
    return object;
}

extern "C" void __fastcall FableSystemInputReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.inputReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void __fastcall FableSystemDriveReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.driveReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" ESystemCode __fastcall FableSystemDriveInitialise_009a6610(
    void*,
    void*,
    void*)
{
    ++g_calls.driveInitialise;
    return g_driveResult;
}

extern "C" void* __fastcall FableSystemProfileConstruct_009a6610(
    void* object,
    void*,
    const void* init,
    fable_u32)
{
    ++g_calls.profileConstruct;
    g_profileObject = object;
    g_observedProfileInit = init;
    return object;
}

extern "C" void __fastcall FableSystemProfileReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.profileReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void* __fastcall FableSystemFontConstruct_009a6610(
    void* object,
    void*,
    const void* first,
    const void* second)
{
    ++g_calls.fontConstruct;
    g_fontObject = object;
    g_observedFontA = first;
    g_observedFontB = second;
    return object;
}

extern "C" void __fastcall FableSystemFontReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.fontReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void* __fastcall
FableSystemTextConstruct_009a6610(void* object, void*)
{
    ++g_calls.textConstruct;
    g_textObject = object;
    return object;
}

extern "C" void __fastcall FableSystemTextReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.textReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void* __fastcall
FableSystemMeshConstruct_009a6610(void* object, void*)
{
    ++g_calls.meshConstruct;
    g_meshObject = object;
    Dword(static_cast<unsigned char*>(object), 4) = 0xCAFEBABE;
    return object;
}

extern "C" void __fastcall FableSystemMeshReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.meshReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void __fastcall
FableSystemAddComponent_009a6610(void*, void*, void*)
{
    ++g_calls.addComponent;
}

extern "C" void* __fastcall
FableSystemColoursConstruct_009a6610(void* object, void*)
{
    ++g_calls.coloursConstruct;
    g_coloursObject = object;
    return object;
}

extern "C" void __fastcall FableSystemColoursReset_009a6610(
    void* slot,
    void*,
    void* object)
{
    ++g_calls.coloursReset;
    *reinterpret_cast<void**>(slot) = object;
}

extern "C" void FableSystemInitialiseCosine_009a6610()
{
    ++g_calls.cosine;
}

extern "C" void FableSystemInitialiseVectorMath_009a6610()
{
    ++g_calls.vectorMath;
}

extern "C" void __fastcall
FableSystemInitColours_009a6610(void* manager, void*)
{
    ++g_calls.initColours;
    static_cast<unsigned char*>(manager)[0xDC] = 1;
}

extern "C" void __fastcall
FableSystemInitApplicationName_009a6610(void*, void*)
{
    ++g_calls.initApplicationName;
}

int main()
{
    const bool early = CheckEarlyBootstrapSkip();
    const bool window = CheckWindowFailure();
    const bool drive = CheckDriveFailurePropagation();
    const bool full = CheckFullSuccess();
    const bool passed = early && window && drive && full;
    if (!passed)
    {
        printf(
            "FABLETLC_SYSTEM_MANAGER_INITIALISE_BEHAVIOR FAIL "
            "early=%u window=%u drive=%u full=%u\n",
            early,
            window,
            drive,
            full);
        return 1;
    }
    printf("FABLETLC_SYSTEM_MANAGER_INITIALISE_BEHAVIOR PASS\n");
    return 0;
}
