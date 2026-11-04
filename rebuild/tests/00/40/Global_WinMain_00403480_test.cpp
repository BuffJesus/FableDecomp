#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_boot.h"

fable_u8* g_FableMicroThreadStackTop_013B7C84 = 0;
fable_u8* g_FableMicroThreadStackBottom_013B7C88 = 0;

namespace
{
    int g_setStackCalls = 0;
    int g_gfMainCalls = 0;
    fable_u8* g_observedStackTop = 0;
    fable_u32 g_observedStackSize = 0;
    FableInstanceHandle g_observedInstance = 0;
    char* g_observedCommandLine = 0;
    int g_observedShowCommand = 0;
}

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(fable_u8* stackTop, unsigned long stackSize)
    {
        ++g_setStackCalls;
        g_observedStackTop = stackTop;
        g_observedStackSize = stackSize;
    }
}

extern "C" long FABLE_FASTCALL GFMain(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand)
{
    ++g_gfMainCalls;
    g_observedInstance = instance;
    g_observedCommandLine = commandLine;
    g_observedShowCommand = showCommand;
    return 0;
}

static int VerifyFirstInstancePath()
{
    char commandLine[] = "-stage1-fixture";
    FableInstanceHandle instance =
        reinterpret_cast<FableInstanceHandle>(0x12345678UL);

    if (WinMain(instance, 0, commandLine, 7) != 0)
        return 1;
    if (g_setStackCalls != 1 || g_gfMainCalls != 1)
        return 2;
    if (g_observedStackSize != 0x00032000UL)
        return 3;
    if (g_observedStackTop != g_FableMicroThreadStackTop_013B7C84)
        return 4;
    if (g_FableMicroThreadStackBottom_013B7C88 == 0)
        return 5;
    if (g_observedStackTop - g_FableMicroThreadStackBottom_013B7C88
        != 0x00032001UL)
        return 6;
    if (g_observedInstance != instance)
        return 7;
    if (g_observedCommandLine != commandLine)
        return 8;
    if (g_observedShowCommand != 7)
        return 9;

    return 0;
}

static int VerifyExistingInstancePath()
{
    const int setStackCalls = g_setStackCalls;
    const int gfMainCalls = g_gfMainCalls;
    char commandLine[] = "-second-instance";

    if (WinMain(0, 0, commandLine, 3) != 0)
        return 10;
    if (g_setStackCalls != setStackCalls)
        return 11;
    if (g_gfMainCalls != gfMainCalls)
        return 12;

    return 0;
}


// Isolate the fixture's mutex from retail and other running bootstrap processes.
// Calls still go through kernel32; only this executable's import slots redirect.
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL GetCurrentProcessId();
extern "C" __declspec(dllimport) int FABLE_STDCALL CloseHandle(void*);
extern "C" char __ImageBase;
namespace {
    typedef void* (FABLE_STDCALL* OpenMutexFunction)(unsigned long, int, const wchar_t*);
    typedef void* (FABLE_STDCALL* CreateMutexFunction)(void*, int, const wchar_t*);
    OpenMutexFunction g_realOpenMutex;
    CreateMutexFunction g_realCreateMutex;
    wchar_t g_fixtureMutex[96];
    void* g_createdMutex;
    void* g_openedMutex;
    bool g_wrongMutexName;
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


    void CheckName(const wchar_t* name) {
        if (!name || wcscmp(name, L"Global\\Fable: The Lost Chapters") != 0)
            g_wrongMutexName = true;
    }
    void* FABLE_STDCALL OpenFixtureMutex(unsigned long access, int inherit, const wchar_t* name) {
        CheckName(name);
        g_openedMutex = g_realOpenMutex(access, inherit, g_fixtureMutex);
        return g_openedMutex;
    }
    void* FABLE_STDCALL CreateFixtureMutex(void* attributes, int owner, const wchar_t* name) {
        CheckName(name);
        g_createdMutex = g_realCreateMutex(attributes, owner, g_fixtureMutex);
        return g_createdMutex;
    }
}

int main()
{
    swprintf(g_fixtureMutex, L"FableTLC-WinMain-fixture-%lu", GetCurrentProcessId());
    if (!SwapImport("OpenMutexW", reinterpret_cast<void*>(&OpenFixtureMutex), reinterpret_cast<void**>(&g_realOpenMutex)) ||
        !SwapImport("CreateMutexW", reinterpret_cast<void*>(&CreateFixtureMutex), reinterpret_cast<void**>(&g_realCreateMutex)))
        return 20;
    int result = VerifyFirstInstancePath();
    if (result == 0)
        result = VerifyExistingInstancePath();

    if (result == 0 && (g_wrongMutexName || !g_createdMutex || !g_openedMutex))
        result = 21;
    if (g_openedMutex) CloseHandle(g_openedMutex);
    if (g_createdMutex) CloseHandle(g_createdMutex);
    SwapImport("OpenMutexW", reinterpret_cast<void*>(g_realOpenMutex), 0);
    SwapImport("CreateMutexW", reinterpret_cast<void*>(g_realCreateMutex), 0);
    if (result != 0)
    {
        printf("FABLETLC_WINMAIN_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_WINMAIN_BEHAVIOR PASS\n");
    return 0;
}
