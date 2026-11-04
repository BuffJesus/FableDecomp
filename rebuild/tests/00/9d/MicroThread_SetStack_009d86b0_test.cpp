#include "fable_microthread.h"
#include <stdio.h>
#include <string.h>
fable_u8* g_FableMicroThreadStackPointer_013CA8A0;
fable_u8* g_FableMicroThreadStackBase_013CA8A4;
unsigned long g_FableMicroThreadStackSize_013CA89C;
int main()
{
    unsigned long buffers[2][8];
    const unsigned long sizes[] = {0x32000UL, 0UL, 0xFFFFFFFFUL, 4UL};
    for (int pass = 0; pass < 4; ++pass)
    {
        memset(buffers, 0xA5, sizeof(buffers));
        const int selected = pass & 1;
        fable_u8* top = reinterpret_cast<fable_u8*>(&buffers[selected][6]);
        MicroThread::SetStack(top, sizes[pass]);
        if (g_FableMicroThreadStackPointer_013CA8A0 != top - 4 ||
            g_FableMicroThreadStackBase_013CA8A4 != top ||
            g_FableMicroThreadStackSize_013CA89C != sizes[pass]) return 1;
        for (int row = 0; row < 2; ++row)
            for (int word = 0; word < 8; ++word)
                if (buffers[row][word] != ((row == selected && word == 5) ? 0xCDCDCDCDUL : 0xA5A5A5A5UL)) return 2;
    }
    puts("MICROTHREAD_SETSTACK_PASS");
    return 0;
}