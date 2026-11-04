#include "fable_system_manager_state.h"
#include "fable_sound_manager.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x180/4], expected[0x180/4];
static unsigned long sound[1],table[4];
static unsigned before,after,mutation,calls,errors;
static CSystemManager* Manager(){return reinterpret_cast<CSystemManager*>(storage);}
static void SetState(unsigned flags) {
    Manager()->WindowsQuit=(flags&1)!=0;
    Manager()->Restore=(flags&2)!=0;
    Manager()->ApplicationActive=(flags&4)!=0;
    Manager()->PSoundManager.Data=(flags&8)?reinterpret_cast<CASoundManager*>(sound):0;
}
void CSystemManager::UpdateActive() {
    if(this!=Manager() || calls++)++errors;
    SetState(after);
    memcpy(expected,storage,sizeof(storage));
    if(!(after&1))reinterpret_cast<unsigned char*>(expected)[0xea]=0;
}
class SoundProbe {
public:
    void Run() {
        if(reinterpret_cast<void*>(this)!=sound || calls++!=1 || memcmp(storage,expected,sizeof(storage)))++errors;
        if(mutation){Manager()->WindowsQuit=true;Manager()->Restore=true;memcpy(expected,storage,sizeof(storage));}
    }
};
int main() {
    void (SoundProbe::*method)()=&SoundProbe::Run;
    FABLE_STATIC_ASSERT(sizeof(method)==sizeof(unsigned long));
    memcpy(table+3,&method,sizeof(method));sound[0]=reinterpret_cast<unsigned long>(table);
    unsigned scenarios=0;
    for(before=0;before<16;++before)for(after=0;after<16;++after)for(mutation=0;mutation<2;++mutation) {
        memset(storage,0xa5,sizeof(storage));SetState(before);calls=errors=0;
        ESystemReturn result=Manager()->Update();++scenarios;
        unsigned wantCalls=1+(!(after&1) && (after&4) && (after&8)?1:0);
        if(errors || calls!=wantCalls || result!=((after&1)?ESR_QUIT:ESR_OK) || memcmp(storage,expected,sizeof(storage))) {
            printf("SYSTEM_UPDATE_FAIL before=%u after=%u mutation=%u\n",before,after,mutation);return 1;
        }
    }
    printf("SYSTEM_UPDATE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
