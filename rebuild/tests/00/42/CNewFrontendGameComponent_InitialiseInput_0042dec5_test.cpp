#include <stdio.h>

struct FableInputPrimitiveFixture
{
    unsigned char unknown00[0x58];
    unsigned long inputObject58;
};

extern "C" unsigned long __fastcall
FableNewFrontendInitialiseInputAddChildPrimitive(
    void*,
    void*)
{
    static FableInputPrimitiveFixture primitive = {};
    primitive.inputObject58 = 0x13579BDFUL;
    return reinterpret_cast<unsigned long>(&primitive);
}

extern "C" void __fastcall
CNewFrontendGameComponent_InitialiseInput_0042dec5(void*, void*);

int main()
{
    unsigned char component[0x6C] = {};
    CNewFrontendGameComponent_InitialiseInput_0042dec5(component, 0);
    if (*reinterpret_cast<unsigned long*>(component + 0x68) !=
        0x13579BDFUL)
    {
        puts("FABLETLC_NEW_FRONTEND_INITIALISE_INPUT FAIL");
        return 1;
    }
    puts("PASS_0042dec5 FABLETLC_NEW_FRONTEND_INITIALISE_INPUT PASS");
    return 0;
}
