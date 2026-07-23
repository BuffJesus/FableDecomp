#include <cstdio>

struct CUserProfileManager {
    char pad[0x134];
    int mField;
};

void __fastcall CUserProfileManager_OnPreDeviceReset(CUserProfileManager* self)
{
    self->mField = 0;
}

int main()
{
    CUserProfileManager obj;
    obj.mField = 0x12345678;
    CUserProfileManager_OnPreDeviceReset(&obj);
    if (obj.mField == 0) {
        std::printf("CUserProfileManager_004067f0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL: mField=%d\n", obj.mField);
    return 1;
}