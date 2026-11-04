#include <stdio.h>
#include <string.h>
#define private public
#include "fable_ime.h"
#undef private

NUISystem::CIME g_FableIME_013B7CD8;
static HWND__* expectedWindow;
static unsigned int expectedMessage, expectedWParam;
static long expectedLParam, helperValue;
static bool helperTrapped, filterValue;
static const tagMSG* expectedFilter;
static int processCalls, toggleCalls, filterCalls, failures;
static int toggleValue;

#define CHECK(x) do { if (!(x)) { ++failures; printf("FAIL line=%d\n", __LINE__); } } while (0)

long FABLE_FASTCALL ImeUi_ProcessMessage(
    HWND__* window, unsigned int message, unsigned int wParam,
    long lParam, bool* trapped)
{
    ++processCalls;
    CHECK(window == expectedWindow);
    CHECK(message == expectedMessage);
    CHECK(wParam == expectedWParam);
    CHECK(lParam == expectedLParam);
    CHECK(!*trapped);
    *trapped = helperTrapped;
    return helperValue;
}

void FABLE_FASTCALL ImeUi_ToggleLanguageBar(int value)
{
    ++toggleCalls;
    CHECK(processCalls == 1);
    toggleValue = value;
}

bool FABLE_FASTCALL ImeUi_IgnoreHotKey(const tagMSG* message)
{
    ++filterCalls;
    CHECK(message == expectedFilter);
    return filterValue;
}

int main()
{
    const unsigned int messages[] = {0, 0x1c, 0x102};
    const unsigned int wParams[] = {0, 1, 0x10000, 0x80000000, 0xffff, 0xffffffff};
    const long values[] = {0, -1, 0x12345678};
    int scenarios = 0;
    for (int initialised = 0; initialised != 2; ++initialised)
    for (int trapped = 0; trapped != 2; ++trapped)
    for (int m = 0; m != 3; ++m)
    for (int w = 0; w != 6; ++w)
    for (int v = 0; v != 3; ++v)
    {
        memset(&g_FableIME_013B7CD8, 0xa5, sizeof(g_FableIME_013B7CD8));
        g_FableIME_013B7CD8.IsInitialised = initialised != 0;
        unsigned char before[sizeof(g_FableIME_013B7CD8)];
        memcpy(before, &g_FableIME_013B7CD8, sizeof(before));
        expectedWindow = reinterpret_cast<HWND__*>(0x12340000);
        expectedMessage = messages[m];
        expectedWParam = wParams[w];
        expectedLParam = values[(v + 1) % 3];
        helperValue = values[v];
        helperTrapped = trapped != 0;
        processCalls = toggleCalls = 0;
        toggleValue = -999;
        long result = 0x76543210;
        bool handled = NUISystem::CIME::ProcessWinMessage(
            expectedWindow, expectedMessage, expectedWParam, expectedLParam, result);
        CHECK(processCalls == initialised);
        bool activation = initialised && !trapped && expectedMessage == 0x1c;
        CHECK(handled == (initialised && (trapped || activation)));
        CHECK(toggleCalls == (activation ? 1 : 0));
        if (activation)
        {
            CHECK(toggleValue == ((expectedWParam & 0xffff) == 0 ? 1 : 0));
            CHECK(result == 0);
        }
        else if (initialised && trapped)
            CHECK(result == helperValue);
        else
            CHECK(result == 0x76543210);
        CHECK(memcmp(before, &g_FableIME_013B7CD8, sizeof(before)) == 0);
        ++scenarios;
    }
    for (int nullMessage = 0; nullMessage != 2; ++nullMessage)
    for (int value = 0; value != 2; ++value)
    {
        tagMSG* message = reinterpret_cast<tagMSG*>(nullMessage ? 0 : 0x45670000);
        expectedFilter = message;
        filterValue = value != 0;
        filterCalls = 0;
        CHECK(NUISystem::CIME::FilterMessage(message) == filterValue);
        CHECK(filterCalls == 1);
        ++scenarios;
    }
    if (failures) return 1;
    printf("IME_MESSAGE_CALLBACKS_BEHAVIOR_PASS scenarios=%d\n", scenarios);
    return 0;
}
