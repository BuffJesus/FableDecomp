// Behaviour fixture for CDateAndTime::CDateAndTime() (0x009E9AB0).
// Links the compiled source object; constructs over dirty storage.
#include "fable_game.h"

#include <new>
#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned char raw[0x1C + 8];
    memset(raw, 0xEE, sizeof(raw));
    CDateAndTime* t = new (raw + 4) CDateAndTime;

    if (t->Year != 0 || t->Month != 0 || t->Day != 0 || t->Hour != 0 ||
        t->Minute != 0 || t->Second != 0 || t->Millisecond != 0) fail |= 1;
    for (int i = 4; i < 4 + 0x1C; ++i)
        if (raw[i] != 0)
            fail |= 2;
    // Exactly seven dwords: the neighbours stay dirty.
    if (raw[3] != 0xEE || raw[4 + 0x1C] != 0xEE) fail |= 4;

    if (fail == 0)
        printf("PASS_009e9ab0\n");
    else
        printf("FAIL_009e9ab0 %d\n", fail);
    return 0;
}
