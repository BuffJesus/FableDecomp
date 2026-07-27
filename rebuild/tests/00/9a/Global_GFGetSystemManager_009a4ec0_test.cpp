#include <cstdio>

struct CSystemManager;
CSystemManager* __fastcall GFGetSystemManager();

int main()
{
    if (GFGetSystemManager() ==
        reinterpret_cast<CSystemManager*>(0x013CA618))
    {
        std::printf("FABLETLC_GET_SYSTEM_MANAGER_BEHAVIOR PASS\n");
        return 0;
    }
    std::printf("FABLETLC_GET_SYSTEM_MANAGER_BEHAVIOR FAIL\n");
    return 1;
}
