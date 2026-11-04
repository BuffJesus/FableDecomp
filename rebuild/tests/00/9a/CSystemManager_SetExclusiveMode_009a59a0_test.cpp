#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

static CSystemManager* manager;
static void* expectedInput;
static CDisplayManager* expectedDisplay;
static int failures, firstFailure, inputMask, mutate, priorityResult, displayResult;
static int priorities, inputCalls, displays, processCalls;
static bool modes[2], inputArguments[2], displayArguments[2];
static char trace[24];
static int traceSize;
#define CHECK(x) do { if (!(x)) { if (!failures) firstFailure = __LINE__; ++failures; } } while (0)
static void Record(char c) { trace[traceSize++] = c; trace[traceSize] = 0; }

static void* FABLE_STDCALL CurrentProcess()
{
    ++processCalls; Record('G');
    return reinterpret_cast<void*>(0x87654321);
}
static int FABLE_STDCALL Priority(void* process, unsigned long priority)
{
    CHECK(process == reinterpret_cast<void*>(0x87654321) && priority == 0x20);
    CHECK(priorities < 2 && manager->ExclusiveMode == modes[priorities]);
    ++priorities; Record('P');
    return priorityResult;
}
static bool FABLE_FASTCALL Input(void* self, void*, bool exclusive)
{
    CHECK(self == expectedInput && inputCalls < 2);
    CHECK(exclusive == modes[priorities-1] && exclusive == manager->ExclusiveMode);
    inputArguments[inputCalls++] = exclusive; Record('I');
    if (mutate) manager->ExclusiveMode = !manager->ExclusiveMode;
    return (inputMask & (1 << (priorities-1))) != 0;
}
CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetExclusiveMode(bool exclusive)
{
    CHECK(this == expectedDisplay && displays < 2);
    CHECK(exclusive == manager->ExclusiveMode);
    displayArguments[displays++] = exclusive; Record('D');
    return static_cast<ECreateDeviceSuccess>(displayResult);
}
extern "C" void* (FABLE_STDCALL *TestProcessSlot)() = CurrentProcess;
extern "C" int (FABLE_STDCALL *TestPrioritySlot)(void*, unsigned long) = Priority;
#pragma comment(linker, "/alternatename:__imp__GetCurrentProcess@0=_TestProcessSlot")
#pragma comment(linker, "/alternatename:__imp__SetPriorityClass@8=_TestPrioritySlot")

// Override any imports also pulled in by the CRT, then restore before exit.
extern "C" char __ImageBase;
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
static void** FindImport(const char* wanted)
{
    unsigned char* image = reinterpret_cast<unsigned char*>(&__ImageBase);
    unsigned char* nt = image + *reinterpret_cast<unsigned long*>(image+0x3c);
    unsigned long* descriptor = reinterpret_cast<unsigned long*>(image + *reinterpret_cast<unsigned long*>(nt+0x80));
    for (; descriptor[0]; descriptor += 5) {
        unsigned long* names = reinterpret_cast<unsigned long*>(image+descriptor[0]);
        void** slots = reinterpret_cast<void**>(image+descriptor[4]);
        for (; *names; ++names, ++slots)
            if (!(*names & 0x80000000) && strcmp(reinterpret_cast<char*>(image+*names+2), wanted) == 0) return slots;
    }
    return 0;
}
static void* Replace(void** slot, void* value)
{
    if (!slot) return 0; // The explicit test alias won; no real import exists.
    unsigned long protection, ignored;
    if (!VirtualProtect(slot, 4, 4, &protection)) { ++failures; return 0; }
    void* previous = *slot; *slot = value;
    VirtualProtect(slot, 4, protection, &ignored);
    return previous;
}

int main()
{
    void** processSlot = FindImport("GetCurrentProcess");
    void** prioritySlot = FindImport("SetPriorityClass");
    void* previousProcess = Replace(processSlot, reinterpret_cast<void*>(CurrentProcess));
    void* previousPriority = Replace(prioritySlot, reinterpret_cast<void*>(Priority));
    if (failures) return 2;
    void* inputTable[3] = {0, reinterpret_cast<void*>(Input), 0};
    void* inputObject[1] = {inputTable};
    expectedInput = inputObject;
    expectedDisplay = reinterpret_cast<CDisplayManager*>(0x12345000);
    int scenarios = 0;
    for (int prevent = 0; prevent != 2; ++prevent)
    for (int previous = 0; previous != 2; ++previous)
    for (int desired = 0; desired != 2; ++desired)
    for (int haveInput = 0; haveInput != 2; ++haveInput)
    for (inputMask = 0; inputMask != 4; ++inputMask)
    for (mutate = 0; mutate != 2; ++mutate)
    for (priorityResult = 0; priorityResult != 2; ++priorityResult)
    for (displayResult = 0; displayResult != 3; ++displayResult)
    {
        unsigned long storage[sizeof(CSystemManager)/4];
        memset(storage, 0xa5, sizeof(storage));
        manager = reinterpret_cast<CSystemManager*>(storage);
        manager->ExclusiveMode = previous != 0;
        manager->PreventExclusiveModeChanges = prevent != 0;
        manager->PInputManager.Data = haveInput ? reinterpret_cast<CInputManager*>(inputObject) : 0;
        manager->PDisplayManager.Data = expectedDisplay;
        unsigned char expected[sizeof(storage)];
        memcpy(expected, storage, sizeof(storage));
        modes[0] = desired != 0; modes[1] = previous != 0;
        priorities = inputCalls = displays = processCalls = traceSize = 0; trace[0] = 0;
        CHECK(!manager->SetExclusiveMode(desired != 0));
        char expectedTrace[24] = "";
        int expectedDisplays = 0;
        if (!prevent) {
            strcat(expectedTrace, "G");
            for (int attempt = 0; attempt != 2; ++attempt) {
                strcat(expectedTrace, "P");
                if (haveInput) {
                    strcat(expectedTrace, "I");
                    CHECK(inputArguments[attempt] == modes[attempt]);
                }
                if (!haveInput || (inputMask & (1 << attempt))) {
                    strcat(expectedTrace, "D");
                    CHECK(displayArguments[expectedDisplays] == (modes[attempt] != (haveInput && mutate)));
                    ++expectedDisplays;
                }
            }
            reinterpret_cast<CSystemManager*>(expected)->ExclusiveMode = (previous != 0) != (haveInput && mutate);
        }
        CHECK(strcmp(trace, expectedTrace) == 0);
        CHECK(processCalls == !prevent && priorities == (prevent ? 0 : 2));
        CHECK(inputCalls == (prevent || !haveInput ? 0 : 2) && displays == expectedDisplays);
        CHECK(memcmp(storage, expected, sizeof(storage)) == 0);
        ++scenarios;
    }
    Replace(processSlot, previousProcess);
    Replace(prioritySlot, previousPriority);
    if (failures) { printf("FAIL count=%d first_line=%d\n", failures, firstFailure); return 1; }
    printf("EXCLUSIVE_MODE_BEHAVIOR_PASS scenarios=%d\n", scenarios);
    return 0;
}
