#include "fable_sound_enabled.h"
#include <stdio.h>
#include <string.h>
static unsigned char storage[0x80];
static unsigned long table[54];
static int calls, failures;
static bool mutate;
struct Callback {
    void Disable(long value) {
        if (this!=reinterpret_cast<Callback*>(storage) || value!=0 || storage[0x25]!=0) ++failures;
        ++calls;
        if (mutate) storage[0x25]=1;
    }
};
int main() {
    void (Callback::*callback)(long)=&Callback::Disable;
    memcpy(table+18,&callback,4);
    for (int scenario=0;scenario<8;++scenario) {
        memset(storage,0xa5,sizeof(storage));
        *reinterpret_cast<unsigned long*>(storage)=reinterpret_cast<unsigned long>(table);
        bool initial=(scenario&1)!=0, desired=(scenario&2)!=0;
        mutate=(scenario&4)!=0;
        storage[0x25]=initial;
        unsigned char before[0x80]; memcpy(before,storage,sizeof(storage));
        calls=0;
        reinterpret_cast<CASoundManager*>(storage)->UnknownSetEnabled_009915e0(desired);
        bool notified=initial && !desired;
        if (calls!=int(notified) || storage[0x25]!=(notified && mutate?1:int(desired))) ++failures;
        for (int i=0;i<sizeof(storage);++i)
            if (i!=0x25 && storage[i]!=before[i]) ++failures;
    }
    printf("SOUND_ENABLED_%s scenarios=8\n",failures?"FAIL":"PASS");
    return failures?1:0;
}
