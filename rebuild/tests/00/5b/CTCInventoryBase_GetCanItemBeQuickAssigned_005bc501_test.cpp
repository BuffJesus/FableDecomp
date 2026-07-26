#include <cstdio>

struct CTCInventoryBase {
    char pad[0x34];
    bool canQuickAssign;
};

bool __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(const CTCInventoryBase* self)
{
    return self->canQuickAssign;
}

int main()
{
    CTCInventoryBase a;
    a.canQuickAssign = true;
    CTCInventoryBase b;
    b.canQuickAssign = false;
    if (CTCInventoryBase_GetCanItemBeQuickAssigned(&a) == true &&
        CTCInventoryBase_GetCanItemBeQuickAssigned(&b) == false) {
        std::printf("CTCInventoryBase_005bc501_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}