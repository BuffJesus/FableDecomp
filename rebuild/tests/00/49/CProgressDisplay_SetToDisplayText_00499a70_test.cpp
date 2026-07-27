#include "fable_boot.h"

#include <stdio.h>
#include <string.h>

const char g_FableEmptyText_0122D70E[] = "";

namespace
{
    int g_AssignmentCalls = 0;
    int g_CalculateCalls = 0;
    const char* g_AssignedText = 0;
}

const CCharString& CCharString::operator=(const char* text)
{
    ++g_AssignmentCalls;
    g_AssignedText = text;
    return *this;
}

void CProgressDisplay::CalculateNextTextTag()
{
    ++g_CalculateCalls;
    value44 = 7;
}

int main()
{
    unsigned char storage[sizeof(CProgressDisplay)] = {0};
    CProgressDisplay* display =
        reinterpret_cast<CProgressDisplay*>(storage);
    display->value44 = 9;

    display->SetToDisplayText(false);
    if (
        display->displayText7C ||
        display->value44 != 0 ||
        g_AssignmentCalls != 1 ||
        g_AssignedText != g_FableEmptyText_0122D70E ||
        g_CalculateCalls != 0)
    {
        return 1;
    }

    display->value44 = 9;
    display->SetToDisplayText(true);
    if (
        !display->displayText7C ||
        display->value44 != 7 ||
        g_AssignmentCalls != 1 ||
        g_CalculateCalls != 1)
    {
        return 1;
    }

    printf("FABLETLC_PROGRESS_DISPLAY_TEXT_MODE_BEHAVIOR PASS\n");
    return 0;
}
