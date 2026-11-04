#include "fable_system_font.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[2][2],expected[2][2];
static unsigned receiver,calls;
void CInitBaseClass::Uninitialise(){
    if(this==reinterpret_cast<CInitBaseClass*>(storage[receiver]))++calls;
    reinterpret_cast<unsigned char*>(storage[receiver])[4]=0;
}
int main(){
    for(unsigned fill=0;fill<256;++fill)for(receiver=0;receiver<2;++receiver){
        memset(storage,fill,sizeof(storage));memcpy(expected,storage,sizeof(storage));reinterpret_cast<unsigned char*>(expected[receiver])[4]=0;calls=0;
        reinterpret_cast<CSystemFont*>(storage[receiver])->Uninitialise();
        if(calls!=1 || memcmp(storage,expected,sizeof(storage)))return 1;
    }
    puts("SYSTEM_FONT_UNINITIALISE_PASS scenarios=512");return 0;
}
