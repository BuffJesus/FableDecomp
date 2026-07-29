#include <stdio.h>

extern "C" bool __fastcall
ReturnFalse_OneArgument_0066fad0(
    void* object, void*, const void* argument);

int main()
{
    unsigned long object = 0x12345678;
    unsigned long argument = 0x87654321;

    if (ReturnFalse_OneArgument_0066fad0(
            &object, 0, &argument))
    {
        puts("FAIL: non-null inputs must return false");
        return 1;
    }

    if (ReturnFalse_OneArgument_0066fad0(0, 0, 0))
    {
        puts("FAIL: null inputs must return false");
        return 1;
    }

    if (object != 0x12345678 || argument != 0x87654321)
    {
        puts("FAIL: stub must not mutate its inputs");
        return 1;
    }

    puts("PASS false-return one-argument stub 0x0066FAD0");
    return 0;
}
