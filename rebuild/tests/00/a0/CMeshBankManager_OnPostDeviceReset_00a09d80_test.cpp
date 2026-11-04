#include "fable_system_manager.h"
#include <stdio.h>

int main()
{
    int cases = 0;
    for (unsigned long p = 0; p != 32; ++p) {
        CMeshBankManager* manager = reinterpret_cast<CMeshBankManager*>(0x10000 + p * 4);
        if (!manager->CMeshBankManager::OnPostDeviceReset()) {
            printf("FAIL case=%lu\n", p);
            return 1;
        }
        ++cases;
    }
    printf("MESH_BANK_POST_RESET_PASS cases=%d\n", cases);
    return 0;
}
