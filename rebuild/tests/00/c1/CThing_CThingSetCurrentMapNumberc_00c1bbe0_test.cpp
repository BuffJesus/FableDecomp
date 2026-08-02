#include <stdio.h>

class CThingImpl {
public:
    unsigned long pad_0x0;
    short pad_0x4;
    short currentMapNumber;

    void __fastcall SetCurrentMapNumber(void* edx_unused, short mapNumber);
};

void __fastcall CThingImpl::SetCurrentMapNumber(void* edx_unused, short mapNumber)
{
    currentMapNumber = mapNumber;
}

int main()
{
    CThingImpl obj;
    obj.pad_0x0 = 0xCCCCCCCC;
    obj.pad_0x4 = (short)0xCCCC;
    obj.currentMapNumber = 0;

    obj.SetCurrentMapNumber(0, (short)1234);

    if (obj.currentMapNumber == 1234 && obj.pad_0x0 == 0xCCCCCCCC)
    {
        printf("TESTOK_SETCURRENTMAPNUMBER\n");
    }
    else
    {
        printf("TESTFAIL\n");
    }

    return 0;
}