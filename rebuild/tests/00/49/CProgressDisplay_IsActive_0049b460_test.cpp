#include <stdio.h>

struct CProgressDisplay
{
    unsigned char unknown00[0x79];
    unsigned char active79;

    bool IsActive() const;
};

int main()
{
    CProgressDisplay display = {};
    if (display.IsActive())
        return 1;

    display.active79 = 1;
    if (!display.IsActive())
        return 2;

    display.active79 = 0x80;
    if (!display.IsActive())
        return 3;

    printf("FABLETLC_PROGRESS_DISPLAY_IS_ACTIVE_BEHAVIOR PASS\n");
    return 0;
}
