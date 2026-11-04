#include "fable_game.h"
#include <new>
#include <stdio.h>
#include <string.h>
int main() {
    unsigned long storage[15];
    memset(storage, 0xA5, sizeof(storage));
    CInterpolationInfo* info = new (storage + 1) CInterpolationInfo;
    for (unsigned i = 1; i != 14; ++i) if (storage[i] != 0) return 1;
    if (storage[0] != 0xA5A5A5A5UL || storage[14] != 0xA5A5A5A5UL) return 2;
    puts("FABLETLC_INTERPOLATION_INFO_CTOR PASS");
    return 0;
}