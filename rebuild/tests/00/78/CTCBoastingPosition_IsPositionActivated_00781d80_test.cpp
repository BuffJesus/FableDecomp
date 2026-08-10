
#include <stdio.h>

struct CTCBoastingPosition {
    unsigned char pad[0x20];
    unsigned char activated;
    long value;
};

static bool IsPositionActivated(CTCBoastingPosition* self, long* out)
{
    if (self->activated) {
        *out = self->value;
    }
    return self->activated != 0;
}

int main()
{
    CTCBoastingPosition p;
    long out;

    /* activated true: writes value, returns true */
    p.activated = 1; p.value = 0x1234; out = -1;
    if (!IsPositionActivated(&p, &out)) { printf("FAIL1\n"); return 1; }
    if (out != 0x1234) { printf("FAIL2\n"); return 1; }

    /* activated false: does NOT write, returns false */
    p.activated = 0; p.value = 0x9999; out = 0x5555;
    if (IsPositionActivated(&p, &out)) { printf("FAIL3\n"); return 1; }
    if (out != 0x5555) { printf("FAIL4\n"); return 1; }

    printf("OK_00781d80\n");
    return 0;
}