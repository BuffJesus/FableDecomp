#include <stdio.h>
#include <string.h>

extern "C" float __cdecl ReturnFloat_1_1_0066fa90();

static unsigned long FloatBits(float value)
{
    unsigned long bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

int main()
{
    const float first = ReturnFloat_1_1_0066fa90();
    const float second = ReturnFloat_1_1_0066fa90();

    if (FloatBits(first) != 0x3f8ccccd)
    {
        printf("FAIL: expected 1.1f bits, got 0x%08lx\n",
               FloatBits(first));
        return 1;
    }

    if (FloatBits(second) != FloatBits(first))
    {
        puts("FAIL: repeated calls must return the same constant");
        return 1;
    }

    puts("PASS fixed float return 0x0066FA90");
    return 0;
}
