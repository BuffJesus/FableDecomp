#include <stdio.h>

// Behaviour model: forward the float to a helper, store result into *dst,
// return dst. Re-implemented standalone.

static int helper(float f) { return (int)(f * 2.0f) + 7; }

static int* setoffset(int* dst, float f)
{
    *dst = helper(f);
    return dst;
}

int main()
{
    int slot = 0;
    int* r = setoffset(&slot, 5.0f);
    // helper(5.0)= 10+7 = 17
    if (r == &slot && slot == 17 && *r == 17)
        printf("BEHAVIOUR_OK\n");
    else
        printf("BEHAVIOUR_BAD\n");
    return 0;
}