// Standalone behaviour test for GetCanItemBeQuickAssigned @ 0x0041cf2c
#include <cstdio>

struct CTCInventoryBase
{
    char _pad[0xC8];
    bool CanItemBeQuickAssigned; // 0xC8
};

bool __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(CTCInventoryBase *self);

int main()
{
    CTCInventoryBase obj;
    obj.CanItemBeQuickAssigned = true;
    bool a = CTCInventoryBase_GetCanItemBeQuickAssigned(&obj);

    obj.CanItemBeQuickAssigned = false;
    bool b = CTCInventoryBase_GetCanItemBeQuickAssigned(&obj);

    if (a == true && b == false)
        printf("CTCINVENTORYBASE_0041cf2c_TEST PASS\n");
    else
        printf("CTCINVENTORYBASE_0041cf2c_TEST FAIL a=%d b=%d\n", (int)a, (int)b);
    return 0;
}
