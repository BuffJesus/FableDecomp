#include "fable_boot.h"

#include <stddef.h>

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableHandle;

struct FableSecurityAttributes;

extern "C" __declspec(dllimport) FableHandle FABLE_STDCALL OpenMutexW(
    FableDword desiredAccess,
    FableBool inheritHandle,
    const wchar_t* name);

extern "C" __declspec(dllimport) FableHandle FABLE_STDCALL CreateMutexW(
    FableSecurityAttributes* attributes,
    FableBool initialOwner,
    const wchar_t* name);

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(fable_u8* stackTop, fable_u32 stackSize);
}

extern "C" long FABLE_FASTCALL GFMain(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand);

namespace
{
    const FableDword kFullMutexAccess = 0x001F0001UL;
    const fable_u32 kMicroThreadStackSize = 0x00032000UL;
    const fable_u32 kStackStorageSize = kMicroThreadStackSize + 1;
    const wchar_t kSingleInstanceMutex[] =
        L"Global\\Fable: The Lost Chapters";

    struct WinMainStackFrame
    {
        fable_u8 alignmentPadding[3];
        fable_u8 microThreadStack[kStackStorageSize];
    };

    FABLE_STATIC_ASSERT(sizeof(WinMainStackFrame) == 0x00032004UL);
}

extern "C" int FABLE_STDCALL WinMain(
    FableInstanceHandle instance,
    FableInstanceHandle /* previousInstance */,
    char* commandLine,
    int showCommand)
{
    WinMainStackFrame stackFrame;

    FableHandle existingInstance =
        OpenMutexW(kFullMutexAccess, 0, kSingleInstanceMutex);
    if (existingInstance == 0)
    {
        // Retail intentionally keeps this mutex handle open for process life.
        CreateMutexW(0, 0, kSingleInstanceMutex);

        g_FableMicroThreadStackTop_013B7C84 =
            stackFrame.microThreadStack + kStackStorageSize;
        g_FableMicroThreadStackBottom_013B7C88 =
            stackFrame.microThreadStack;
        MicroThread::SetStack(
            g_FableMicroThreadStackTop_013B7C84,
            kMicroThreadStackSize);
        GFMain(instance, commandLine, showCommand);
    }

    return 0;
}
