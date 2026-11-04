// Behaviour fixture for CConsole::ProcessInput (0x009EA530).
// Links the compiled source object; called on raw console storage.
#include "fable_console.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned char raw[sizeof(CConsole)];
    memset(raw, 0x6B, sizeof(raw));
    CConsole* console = reinterpret_cast<CConsole*>(raw);
    unsigned char eventRaw[64];
    memset(eventRaw, 0x11, sizeof(eventRaw));
    const CInputEvent& event = *reinterpret_cast<const CInputEvent*>(eventRaw);

    // Never consumes the event, whatever it is.
    if (console->ProcessInput(event) != false) fail |= 1;
    memset(eventRaw, 0, sizeof(eventRaw));
    if (console->ProcessInput(event)) fail |= 2;
    // Neither the console nor the event is touched.
    for (unsigned i = 0; i < sizeof(raw); ++i)
        if (raw[i] != 0x6B)
            fail |= 4;
    for (unsigned j = 0; j < sizeof(eventRaw); ++j)
        if (eventRaw[j] != 0)
            fail |= 8;

    if (fail == 0)
        printf("AUTO_TINY_009ea530_TEST PASS\n");
    else
        printf("AUTO_TINY_009ea530_TEST FAIL %d\n", fail);
    return 0;
}
