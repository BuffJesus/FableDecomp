#include "fable_world.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

int main() {
    unsigned direct=0, composed=0, sequential=0;
    for(unsigned pattern=0;pattern<4;++pattern)
    for(unsigned start=0;start<32;++start)
    for(unsigned count=0;count<=32;++count)
    for(unsigned state=0;state<4;++state) {
        CGameEventDataPackage package;
        for(unsigned i=0;i<32;++i) package.Data[i]=(i*37+pattern*71)&255;
        const long lengths[4]={0,32,-1,0x7fffffff};
        package.Length=lengths[state];package.StartPos=start;package.EndPos=state*9;
        CGameEventDataPackage before;memcpy(&before,&package,sizeof(package));
        unsigned char output[48],expected[48];memset(output,0xa7,sizeof(output));memcpy(expected,output,sizeof(output));
        for(i=0;i<count;++i) expected[8+i]=before.Data[(start+i)%32];
        package.GetData(count,output+8);++direct;
        before.Length-=count;before.StartPos=(start+count)%32;
        if(memcmp(&package,&before,sizeof(package)) || memcmp(output,expected,sizeof(output))) return 1;
    }
    for(unsigned start=0;start<32;++start) {
        CGameEventDataPackage package;package.Length=32;package.StartPos=start;package.EndPos=17;
        for(unsigned i=0;i<32;++i) package.Data[i]=i*7;
        unsigned char data[32];
        for(i=0;i<32;++i) {data[i]=package.GetUBYTE();++sequential;}
        for(i=0;i<32;++i) if(data[i]!=((start+i)%32)*7) return 2;
        if(package.StartPos!=start || package.Length!=0 || package.EndPos!=17) return 3;
    }
    for(unsigned value=0;value<256;++value)
    for(unsigned length=1;length<=32;++length) {
        unsigned long storage[104];memset(storage,255-value,sizeof(storage));
        CWorld* world=reinterpret_cast<CWorld*>(&storage[2]);
        CGameEvent event(0x7fffffff);memset(event.Data,value,sizeof(event.Data));event.EndPos=length;
        CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));
        unsigned char before[sizeof(storage)];memcpy(before,storage,sizeof(storage));
        world->EASetPauseMode(&event);++composed;before[8+0xa4]=value==1?1:0;
        if(memcmp(before,storage,sizeof(storage)) || memcmp(&event,&eventBefore,sizeof(event))) return 4;
        world->EASetSlowMotion(&event);++composed;before[8+0xa5]=value==1?1:0;
        if(memcmp(before,storage,sizeof(storage)) || memcmp(&event,&eventBefore,sizeof(event))) return 5;
    }
    printf("EVENT_READER_BEHAVIOR_PASS direct=%u sequential=%u composed=%u\n",direct,sequential,composed);return 0;
}
