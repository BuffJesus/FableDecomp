#include "fable_system_manager.h"
#include <string.h>
#include <stdio.h>
struct CSystemManagerTestAccess { static void Run(CSystemManager* p,bool focus) { p->SetHasInputFocus(focus); } };
static CSystemManager* manager;
static unsigned long input[0x60/4];
static unsigned calls,errors;
static bool desired,mutate;
void CInputManager::SetHasInputFocus(bool focus) {
    ++calls;
    if(this!=reinterpret_cast<CInputManager*>(input) || focus!=desired || manager->ApplicationHasInputFocus!=focus) ++errors;
    if(mutate) manager->ApplicationHasInputFocus=!focus;
}
int main() {
    for(unsigned flags=0;flags<16;++flags) {
        unsigned long storage[0x180/4];memset(storage,0xa5,sizeof(storage));
        manager=reinterpret_cast<CSystemManager*>(storage);
        desired=(flags&1)!=0;mutate=(flags&8)!=0;bool haveInput=(flags&4)!=0;
        manager->ApplicationHasInputFocus=(flags&2)!=0;
        manager->PInputManager.Data=haveInput?reinterpret_cast<CInputManager*>(input):0;
        unsigned char expected[0x180];memcpy(expected,storage,sizeof(expected));
        expected[10]=haveInput && mutate?!desired:desired;
        calls=0;CSystemManagerTestAccess::Run(manager,desired);
        if(errors || calls!=unsigned(haveInput) || memcmp(storage,expected,sizeof(expected))) { printf("SYSTEM_FOCUS_FAIL scenario=%u\n",flags);return 1; }
    }
    puts("SYSTEM_FOCUS_PASS scenarios=16");return 0;
}
