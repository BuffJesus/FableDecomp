#include "fable_boot.h"

void CProgressDisplay::SetToDisplayText(bool enabled)
{
    displayText7C = enabled;
    if (enabled)
    {
        CalculateNextTextTag();
        return;
    }

    primaryText4C = g_FableEmptyText_0122D70E;
    value44 = 0;
}
