#include <stdio.h>
#include <string.h>

#include "fable_boot.h"

fable_u8 g_CProgressDisplayVTable_012388B4 = 0;
double g_FableZeroDouble_0122ED70 = 0.0;

void* operator new(size_t, void* address)
{
    return address;
}

namespace
{
    int g_wideConstructCalls = 0;
    int g_charConstructCalls = 0;
    void* g_wideAddress = 0;
    void* g_firstCharAddress = 0;
    void* g_secondCharAddress = 0;
}

void FABLE_FASTCALL FableConstructWideString(CWideString* value)
{
    ++g_wideConstructCalls;
    g_wideAddress = value;
}

void FABLE_FASTCALL FableConstructCharString(CCharString* value)
{
    ++g_charConstructCalls;
    if (g_firstCharAddress == 0)
        g_firstCharAddress = value;
    else
        g_secondCharAddress = value;
}

int main()
{
    unsigned char storage[sizeof(CProgressDisplay)];
    memset(storage, 0xA5, sizeof(storage));

    CProgressDisplay* display =
        ::new (storage) CProgressDisplay;

    if (
        display->vftable00 != &g_CProgressDisplayVTable_012388B4 ||
        display->value04 != 0 ||
        display->value44 != 0 ||
        display->value54 != 0 ||
        display->value58 != 0)
    {
        return 1;
    }
    if (
        g_wideConstructCalls != 1 ||
        g_charConstructCalls != 2 ||
        g_wideAddress != storage + 0x48 ||
        g_firstCharAddress != storage + 0x4C ||
        g_secondCharAddress != storage + 0x50)
    {
        return 2;
    }
    if (
        display->progressTime60 != 0.0 ||
        display->displayTime68 != 0.0 ||
        display->completionTime70 != 0.0 ||
        display->flag78 != 0 ||
        display->active79 != 0 ||
        display->drawRetail7A != 1 ||
        display->flag7B != 0 ||
        display->displayText7C != 1 ||
        display->flag7D != 0 ||
        display->flag7E != 0 ||
        display->flag7F != 0 ||
        display->flag80 != 0 ||
        display->flag81 != 0)
    {
        return 3;
    }
    if (
        storage[0x5C] != 0xA5 ||
        storage[0x82] != 0xA5 ||
        storage[0x87] != 0xA5)
    {
        return 4;
    }

    printf("FABLETLC_PROGRESS_DISPLAY_CONSTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
