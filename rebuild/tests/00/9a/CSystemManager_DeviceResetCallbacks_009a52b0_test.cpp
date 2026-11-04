#include "fable_system_manager_state.h"
#include <stdio.h>
#include <string.h>
int main(){
    unsigned long storage[0x180/4],expected[0x180/4];
    for(unsigned fill=0;fill<256;++fill){
        memset(storage,fill,sizeof(storage));memcpy(expected,storage,sizeof(storage));
        CSystemManager* manager=reinterpret_cast<CSystemManager*>(storage);
        manager->OnPreDeviceReset();if(!manager->OnPostDeviceReset() || memcmp(storage,expected,sizeof(storage)))return 1;
    }
    puts("SYSTEM_RESET_CALLBACKS_PASS scenarios=256");return 0;
}
