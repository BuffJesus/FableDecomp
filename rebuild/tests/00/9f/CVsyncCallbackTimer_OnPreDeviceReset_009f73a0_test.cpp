#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[2][16],expected[2][16];
static unsigned slot,calls;
void CVsyncCallbackTimer::ClearAllQueries(){
    if(this==reinterpret_cast<CVsyncCallbackTimer*>(storage[slot]))++calls;
    storage[slot][3]=0x12345678;
}
int main(){
    for(unsigned fill=0;fill<256;++fill)for(slot=0;slot<2;++slot){
        memset(storage,fill,sizeof(storage));memcpy(expected,storage,sizeof(storage));expected[slot][3]=0x12345678;calls=0;
        reinterpret_cast<CVsyncCallbackTimer*>(storage[slot])->OnPreDeviceReset();
        if(calls!=1 || memcmp(storage,expected,sizeof(storage)))return 1;
    }
    puts("TIMER_PRE_RESET_PASS scenarios=512");return 0;
}
