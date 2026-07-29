#include <stdio.h>
#include <string.h>

extern "C" float __cdecl ReturnFloat_0_5_0066fa70();

static unsigned long FloatBits(float value)
{
    unsigned long bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

int main()
{
    const float first = ReturnFloat_0_5_0066fa70();
    const float second = ReturnFloat_0_5_0066fa70();

    if (FloatBits(first) != 0x3f000000)
    {
        printf("FAIL: expected 0.5f bits, got 0x%08lx\n",
               FloatBits(first));
        return 1;
    }

    if (FloatBits(second) != FloatBits(first))
    {
        puts("FAIL: repeated calls must return the same constant");
        return 1;
    }

    puts("PASS fixed float return 0x0066FA70");
    return 0;
}
