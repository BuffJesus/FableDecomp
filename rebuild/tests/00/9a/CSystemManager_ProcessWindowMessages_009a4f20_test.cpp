#include "fable_system_manager_state.h"
#include "fable_window_message.h"
#include <stdio.h>
#include <string.h>

struct CSystemManagerTestAccess
{
    static void Run(CSystemManager* manager) { manager->ProcessWindowMessages(); }
};

static CSystemManager* manager;
static int failures, firstFailure, count, nextMessage, mode, changes, lockDepth;
static tagMSG queue[3];
static char trace[64];
static int traceSize;
static void* expectedWindow;
#define CHECK(x) do { if (!(x)) { if (!failures) firstFailure = __LINE__; ++failures; } } while (0)
static void Record(char c) { trace[traceSize++] = c; trace[traceSize] = 0; }
static bool FABLE_FASTCALL Filter(tagMSG* message)
{
    CHECK(lockDepth == 1);
    CHECK(memcmp(message, &queue[nextMessage - 1], sizeof(*message)) == 0);
    Record('F');
    if (changes) {
        expectedWindow = reinterpret_cast<void*>(0x87654321);
        manager->ApplicationWinHandle = expectedWindow;
        message->wParam ^= 0x80000000;
    }
    if (changes == 2) manager->MessageFilter = 0;
    return mode == 2 || (mode == 3 && (message->message & 1));
}
static void FABLE_STDCALL Enter(void* section)
{
    CHECK(section == manager->CriticalSection);
    CHECK(lockDepth == 0); ++lockDepth; Record('E');
}
static void FABLE_STDCALL Leave(void* section)
{
    CHECK(section == manager->CriticalSection);
    CHECK(lockDepth == 1); --lockDepth; Record('L');
}
static int FABLE_STDCALL Peek(tagMSG* message, HWND__* window,
                            unsigned int minimum, unsigned int maximum, unsigned int remove)
{
    CHECK(lockDepth == 1);
    CHECK(window == expectedWindow);
    CHECK(minimum == 0 && maximum == 0 && remove == 1);
    Record('P');
    if (nextMessage == count) return 0;
    *message = queue[nextMessage++];
    return 1;
}
static void CheckDelivered(const tagMSG* message)
{
    tagMSG expected = queue[nextMessage - 1];
    if (changes && mode && (changes != 2 || nextMessage == 1)) expected.wParam ^= 0x80000000;
    CHECK(memcmp(message, &expected, sizeof(expected)) == 0);
    CHECK(lockDepth == 1);
}
static int FABLE_STDCALL Translate(const tagMSG* message)
{
    CheckDelivered(message); Record('T');
    return nextMessage & 1; // Both outcomes must still dispatch.
}
static long FABLE_STDCALL Dispatch(const tagMSG* message)
{
    CheckDelivered(message); Record('D');
    return -1; // Dispatch's result does not stop the queue.
}
extern "C" {
void (FABLE_STDCALL *TestEnterSlot)(void*) = Enter;
void (FABLE_STDCALL *TestLeaveSlot)(void*) = Leave;
int (FABLE_STDCALL *TestPeekSlot)(tagMSG*, HWND__*, unsigned int, unsigned int, unsigned int) = Peek;
int (FABLE_STDCALL *TestTranslateSlot)(const tagMSG*) = Translate;
long (FABLE_STDCALL *TestDispatchSlot)(const tagMSG*) = Dispatch;
}
#pragma comment(linker, "/alternatename:__imp__EnterCriticalSection@4=_TestEnterSlot")
#pragma comment(linker, "/alternatename:__imp__LeaveCriticalSection@4=_TestLeaveSlot")
#pragma comment(linker, "/alternatename:__imp__PeekMessageW@20=_TestPeekSlot")
#pragma comment(linker, "/alternatename:__imp__TranslateMessage@4=_TestTranslateSlot")
#pragma comment(linker, "/alternatename:__imp__DispatchMessageW@4=_TestDispatchSlot")

// The CRT also imports these kernel32 functions, so /alternatename cannot
// override them. Patch this test process's two IAT slots before exercising the
// unchanged candidate object. Restore them before returning to the CRT.
extern "C" __declspec(dllimport) void* FABLE_STDCALL GetModuleHandleA(const char*);
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
static void** FindImport(const char* wanted)
{
    unsigned char* image = static_cast<unsigned char*>(GetModuleHandleA(0));
    unsigned char* nt = image + *reinterpret_cast<unsigned long*>(image + 0x3c);
    unsigned long* descriptor = reinterpret_cast<unsigned long*>(image + *reinterpret_cast<unsigned long*>(nt + 0x80));
    for (; descriptor[0]; descriptor += 5) {
        unsigned long* names = reinterpret_cast<unsigned long*>(image + descriptor[0]);
        void** slots = reinterpret_cast<void**>(image + descriptor[4]);
        for (; *names; ++names, ++slots)
            if (!(*names & 0x80000000) && strcmp(reinterpret_cast<char*>(image + *names + 2), wanted) == 0)
                return slots;
    }
    return 0;
}
static void* ReplaceImport(void** slot, void* replacement)
{
    unsigned long protection, ignored;
    if (!VirtualProtect(slot, sizeof(*slot), 4, &protection)) return 0;
    void* old = *slot;
    *slot = replacement;
    VirtualProtect(slot, sizeof(*slot), protection, &ignored);
    return old;
}

int main()
{
    void** enterSlot = FindImport("EnterCriticalSection");
    void** leaveSlot = FindImport("LeaveCriticalSection");
    if (!enterSlot || !leaveSlot) return 2;
    void* savedEnter = ReplaceImport(enterSlot, reinterpret_cast<void*>(Enter));
    void* savedLeave = ReplaceImport(leaveSlot, reinterpret_cast<void*>(Leave));
    if (!savedEnter || !savedLeave) return 3;
    unsigned char storage[sizeof(CSystemManager)];
    int scenarios = 0;
    for (mode = 0; mode != 4; ++mode)
    for (count = 0; count != 4; ++count)
    for (changes = 0; changes != 3; ++changes)
    for (int nullWindow = 0; nullWindow != 2; ++nullWindow)
    {
        memset(storage, 0xa5, sizeof(storage));
        manager = reinterpret_cast<CSystemManager*>(storage);
        manager->MessageFilter = mode ? Filter : 0;
        expectedWindow = reinterpret_cast<void*>(nullWindow ? 0 : 0x12345678);
        manager->ApplicationWinHandle = expectedWindow;
        nextMessage = traceSize = lockDepth = 0;
        trace[0] = 0;
        for (int i = 0; i != 3; ++i) {
            memset(&queue[i], 0, sizeof(queue[i]));
            queue[i].hwnd = reinterpret_cast<HWND__*>(0x10203040 + i);
            queue[i].message = i == 2 ? 0x12 : 0x100 + i; // Includes WM_QUIT.
            queue[i].wParam = 0xfedcba98 + i;
            queue[i].lParam = -17 - i;
            queue[i].time = 0x98765432 + i;
            queue[i].pt.x = -100; queue[i].pt.y = 200;
        }
        unsigned char before[sizeof(storage)];
        memcpy(before, storage, sizeof(storage));
        CSystemManagerTestAccess::Run(manager);
        char expectedTrace[64] = "EP";
        for (int j = 0; j != count; ++j) {
            bool filtered = mode && (changes != 2 || j == 0);
            if (filtered) strcat(expectedTrace, "F");
            if (!filtered || !(mode == 2 || (mode == 3 && (queue[j].message & 1))))
                strcat(expectedTrace, "TD");
            strcat(expectedTrace, "P");
        }
        strcat(expectedTrace, "L");
        CHECK(strcmp(trace, expectedTrace) == 0);
        CHECK(lockDepth == 0 && nextMessage == count);
        if (mode && changes && count) {
            reinterpret_cast<CSystemManager*>(before)->ApplicationWinHandle = expectedWindow;
            if (changes == 2) reinterpret_cast<CSystemManager*>(before)->MessageFilter = 0;
        }
        CHECK(memcmp(before, storage, sizeof(storage)) == 0);
        ++scenarios;
    }
    ReplaceImport(enterSlot, savedEnter);
    ReplaceImport(leaveSlot, savedLeave);
    if (failures) { printf("FAIL count=%d first_line=%d\n", failures, firstFailure); return 1; }
    printf("WINDOW_MESSAGE_LOOP_BEHAVIOR_PASS scenarios=%d\n", scenarios);
    return 0;
}
