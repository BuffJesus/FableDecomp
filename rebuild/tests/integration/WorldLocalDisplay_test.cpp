#include "fable_world.h"
#include "fable_game.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>
static CMainGameComponent* expectedComponent;static unsigned long expectedWidth,expectedHeight;static unsigned char expectedDepth;static unsigned calls,errors;
void CMainGameComponent::SetDisplayMode(unsigned long width,unsigned long height,unsigned char depth) {if(this!=expectedComponent || width!=expectedWidth || height!=expectedHeight || depth!=expectedDepth) ++errors;++calls;}
static int Run(unsigned short width,unsigned short height,unsigned char depth,unsigned& scenarios) {
    for(unsigned pattern=0;pattern<4;++pattern) {
        unsigned long storage[104];memset(storage,pattern*71,sizeof(storage));CWorld* world=reinterpret_cast<CWorld*>(&storage[2]);expectedComponent=reinterpret_cast<CMainGameComponent*>(0x12345678+pattern*0x1000);memcpy((unsigned char*)world+8,&expectedComponent,4);unsigned long before[104];memcpy(before,storage,sizeof(before));
        CGameEvent event(0x7fffffff);memset(event.Data,pattern*71,sizeof(event.Data));memcpy(&event.Data[0],&width,2);memcpy(&event.Data[2],&height,2);event.Data[4]=depth;expectedWidth=width;expectedHeight=height;expectedDepth=depth;
        for(unsigned length=5;length<=32;++length) {
            event.EndPos=length;CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));calls=errors=0;world->LEASetDisplayMode(&event);++scenarios;
            if(calls!=1 || errors || memcmp(before,storage,sizeof(before)) || memcmp(&event,&eventBefore,sizeof(event))) return 1;
        }
    }
    return 0;
}
int main() {
    unsigned scenarios=0;static const unsigned short dims[8]={0,1,255,256,0x8000,0xffff,1280,1920};static const unsigned char depths[6]={0,1,16,24,32,255};
    for(unsigned x=0;x<8;++x) for(unsigned y=0;y<8;++y) for(unsigned d=0;d<6;++d) if(Run(dims[x],dims[y],depths[d],scenarios)) return 1;
    for(unsigned d=0;d<256;++d) if(Run(1280,720,(unsigned char)d,scenarios)) return 2;
    printf("WORLD_LOCAL_DISPLAY_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
