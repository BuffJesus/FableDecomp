#include "engine/CTCBoastingPosition.h"
#include <stdio.h>
#include <string.h>

static bool IsPositionActivated(CTCBoastingPosition* self, long* out)
{
    if (self->PositionActivated) {
        *out = self->BoastIndex;
    }
    return self->PositionActivated != 0;
}

int main()
{
    CTCBoastingPosition p;
    long out;
    memset(&p, 0, sizeof(p));

    p.PositionActivated = 1; p.BoastIndex = 0x1234; out = -1;
    if (!IsPositionActivated(&p, &out)) { printf("FAIL1\n"); return 1; }
    if (out != 0x1234) { printf("FAIL2\n"); return 1; }

    p.PositionActivated = 0; p.BoastIndex = 0x9999; out = 0x5555;
    if (IsPositionActivated(&p, &out)) { printf("FAIL3\n"); return 1; }
    if (out != 0x5555) { printf("FAIL4\n"); return 1; }

    printf("OK_00781d80\n");
    return 0;
}
