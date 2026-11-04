// Behaviour fixture for CGameEvent::AddData (0x009F1760).
// Links the compiled source object.
#include "fable_game_event.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    CGameEvent event(5);
    memset(event.Data, 0xEE, sizeof(event.Data));
    event.Player = 3;

    // Appends at the cursor and advances it.
    unsigned long value = 0x11223344;
    event.AddData(4, &value);
    if (event.EndPos != 4 || memcmp(event.Data, &value, 4) != 0 || event.Data[4] != 0xEE) fail |= 1;
    const char text[7] = "abcdef";
    event.AddData(7, text);
    if (event.EndPos != 11 || memcmp(event.Data + 4, text, 7) != 0 || event.Data[11] != 0xEE) fail |= 2;
    // The inline AddUBYTE goes through it too.
    event.AddUBYTE(0x9C);
    if (event.EndPos != 12 || event.Data[11] != 0x9C) fail |= 4;
    // Zero-length: nothing written, cursor unchanged.
    event.AddData(0, text);
    if (event.EndPos != 12 || event.Data[12] != 0xEE) fail |= 8;
    // Neighbouring fields untouched.
    if (event.Type != 5 || event.Player != 3 || !event.Valid || event.Replacement) fail |= 16;
    // Fill to exactly the end of the payload.
    unsigned char block[20];
    memset(block, 0x42, sizeof(block));
    event.AddData(20, block);
    if (event.EndPos != 32 || event.Data[31] != 0x42 || !event.Valid) fail |= 32;

    if (fail == 0)
        printf("CGAMEEVENT_ADDDATA_PASS\n");
    else
        printf("CGAMEEVENT_ADDDATA_FAIL %d\n", fail);
    return 0;
}