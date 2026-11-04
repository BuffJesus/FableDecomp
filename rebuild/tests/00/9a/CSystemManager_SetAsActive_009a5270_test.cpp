#include "fable_system_manager_state.h"
#include <string.h>
#include <stdio.h>
struct CSystemManagerTestAccess { static void Run(CSystemManager* p,bool active) { p->SetAsActive(active); } };
int main() {
    for(unsigned flags=0;flags<16;++flags) {
        unsigned long storage[0x180/4];memset(storage,0xa5,sizeof(storage));
        CSystemManager* manager=reinterpret_cast<CSystemManager*>(storage);
        bool active=(flags&1)!=0,prior=(flags&2)!=0,ever=(flags&4)!=0,restore=(flags&8)!=0;
        manager->ApplicationActive=prior;manager->EverBeenActive=ever;manager->Restore=restore;
        unsigned char expected[0x180];memcpy(expected,storage,sizeof(expected));
        expected[9]=active;expected[0xdc]=1;expected[0xea]=restore||(active && !prior && ever);
        CSystemManagerTestAccess::Run(manager,active);
        if(memcmp(storage,expected,sizeof(expected))) { printf("ACTIVE_STATE_FAIL scenario=%u\n",flags);return 1; }
    }
    puts("ACTIVE_STATE_PASS scenarios=16");return 0;
}
