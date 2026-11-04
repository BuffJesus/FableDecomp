// Behaviour fixture for CInputManager::SetHasInputFocus (0x009F4E20), linked
// with only the candidate. Devices are raw objects whose vptr points at a
// recording table (slot 14 mouse, 3 keyboard, 16 joystick), so no device
// class body or base vtable is needed. Callbacks mutate the manager mid-call to
// pin what retail rereads (keyboard pointer, joystick begin) and what it
// snapshots (the joystick count).
#include "fable_input_manager.h"
#include <stdio.h>
#include <string.h>

struct CInputManagerTestAccess
{
    static void SetMouse(CInputManager* m, CMouse* p) { *reinterpret_cast<CMouse**>(&m->PMouse) = p; }
    static void SetKeyboard(CInputManager* m, CKeyboard* p) { *reinterpret_cast<CKeyboard**>(&m->PKeyboard) = p; }
    static void SetJoysticks(CInputManager* m, CCountedPointer<CJoystick>* begin, long count)
    {
        m->PJoysticks._M_start = begin;
        m->PJoysticks._M_finish = begin + count;
        m->PJoysticks._M_end_of_storage._M_data = begin + 3;
    }
};
typedef CInputManagerTestAccess Access;

struct FakeDevice { void** vtable; unsigned long body[3]; };
static CInputManager* manager;
static bool expectedFocus;
static int mutation, count, failures, visits;
static FakeDevice mouse, keyboard, alternateKeyboard, joystick[6];
static unsigned long originalSlots[6], alternateSlots[6];
static CCountedPointer<CJoystick>* original = reinterpret_cast<CCountedPointer<CJoystick>*>(originalSlots);
static CCountedPointer<CJoystick>* alternate = reinterpret_cast<CCountedPointer<CJoystick>*>(alternateSlots);
static void* mouseTable[15];
static void* keyboardTable[4];
static void* joystickTable[17];
static char trace[16];
static int traceSize;
static unsigned char expectedStorage[sizeof(CInputManager)];

#define CHECK(x) do { if (!(x)) { ++failures; printf("FAIL line=%d\n", __LINE__); } } while (0)
static void Record(char c) { trace[traceSize++] = c; trace[traceSize] = 0; }
// A fastcall callback with an unused EDX argument receives the same ECX
// object and stack bool as each real thiscall vtable method.
static void FABLE_FASTCALL Mouse(FakeDevice* self, void*, bool focus)
{
    CHECK(self == &mouse && focus == expectedFocus); Record('M');
    if (mutation == 1) {
        Access::SetKeyboard(manager, reinterpret_cast<CKeyboard*>(&alternateKeyboard));
        Access::SetKeyboard(reinterpret_cast<CInputManager*>(expectedStorage), reinterpret_cast<CKeyboard*>(&alternateKeyboard));
    }
}
static void FABLE_FASTCALL Keyboard(FakeDevice* self, void*, bool focus)
{
    CHECK((self == &keyboard || self == &alternateKeyboard) && focus == expectedFocus);
    Record(self == &keyboard ? 'K' : 'k');
    if (mutation == 2) {
        Access::SetJoysticks(manager, alternate, count);
        Access::SetJoysticks(reinterpret_cast<CInputManager*>(expectedStorage), alternate, count);
    }
}
static void FABLE_FASTCALL Joystick(FakeDevice* self, void*, bool focus)
{
    CHECK(self >= joystick && self < joystick + 6 && focus == expectedFocus);
    Record(static_cast<char>('0' + (self - joystick)));
    if (mutation == 3 && visits == 0) {
        // Retail snapshots count once but reloads the vector's begin pointer.
        Access::SetJoysticks(manager, alternate, 0);
        Access::SetJoysticks(reinterpret_cast<CInputManager*>(expectedStorage), alternate, 0);
    }
    ++visits;
}

int main()
{
    mouseTable[14] = reinterpret_cast<void*>(Mouse);
    keyboardTable[3] = reinterpret_cast<void*>(Keyboard);
    joystickTable[16] = reinterpret_cast<void*>(Joystick);
    mouse.vtable = mouseTable;
    keyboard.vtable = alternateKeyboard.vtable = keyboardTable;
    for (int j = 0; j != 6; ++j) joystick[j].vtable = joystickTable;
    for (int j = 0; j != 3; ++j) {
        originalSlots[j*2] = reinterpret_cast<unsigned long>(&joystick[j]);
        originalSlots[j*2+1] = 0xdead0000 + j;
        alternateSlots[j*2] = reinterpret_cast<unsigned long>(&joystick[j+3]);
        alternateSlots[j*2+1] = 0xbeef0000 + j;
    }
    int scenarios = 0;
    for (int focused = 0; focused != 2; ++focused)
    for (int haveMouse = 0; haveMouse != 2; ++haveMouse)
    for (int haveKeyboard = 0; haveKeyboard != 2; ++haveKeyboard)
    for (count = 0; count != 4; ++count)
    for (mutation = 0; mutation != 4; ++mutation)
    {
        unsigned long storage[sizeof(CInputManager)/sizeof(unsigned long)];
        memset(storage, 0xa5, sizeof(storage));
        manager = reinterpret_cast<CInputManager*>(storage);
        Access::SetMouse(manager, haveMouse ? reinterpret_cast<CMouse*>(&mouse) : 0);
        Access::SetKeyboard(manager, haveKeyboard ? reinterpret_cast<CKeyboard*>(&keyboard) : 0);
        Access::SetJoysticks(manager, original, count);
        memcpy(expectedStorage, storage, sizeof(storage));
        expectedFocus = focused != 0;
        traceSize = visits = 0; trace[0] = 0;
        manager->SetHasInputFocus(expectedFocus);
        char expected[16] = "";
        if (haveMouse) strcat(expected, "M");
        bool changedKeyboard = haveMouse && mutation == 1;
        bool calledKeyboard = haveKeyboard || changedKeyboard;
        if (calledKeyboard) strcat(expected, changedKeyboard ? "k" : "K");
        int at = static_cast<int>(strlen(expected));
        for (int k = 0; k != count; ++k) {
            bool changed = (mutation == 2 && calledKeyboard) || (mutation == 3 && k != 0);
            expected[at++] = static_cast<char>('0' + k + (changed ? 3 : 0));
        }
        expected[at] = 0;
        CHECK(strcmp(trace, expected) == 0 && visits == count);
        CHECK(memcmp(storage, expectedStorage, sizeof(storage)) == 0);
        for (int k = 0; k != 3; ++k) {
            CHECK(originalSlots[k*2+1] == 0xdead0000u + k);
            CHECK(alternateSlots[k*2+1] == 0xbeef0000u + k);
        }
        ++scenarios;
    }
    if (failures) return 1;
    printf("INPUT_FOCUS_BEHAVIOR_PASS scenarios=%d\n", scenarios);
    return 0;
}
