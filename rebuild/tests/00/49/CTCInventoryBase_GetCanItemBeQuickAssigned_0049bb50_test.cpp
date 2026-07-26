#include <cstdio>

struct CTCInventoryBase {
    char pad[8];
    bool canQuickAssign;
};

bool __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(const CTCInventoryBase* self) {
    return self->canQuickAssign;
}

int main() {
    CTCInventoryBase a;
    a.canQuickAssign = true;
    CTCInventoryBase b;
    b.canQuickAssign = false;
    if (CTCInventoryBase_GetCanItemBeQuickAssigned(&a) == true &&
        CTCInventoryBase_GetCanItemBeQuickAssigned(&b) == false) {
        std::printf("CTCInventoryBase_0049bb50_TEST PASS\n");
        return 0;
    }
    std::printf("CTCInventoryBase_0049bb50_TEST FAIL\n");
    return 1;
}