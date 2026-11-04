#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineInternalPrimitive2DSprite.h"
#include "engine/CEngineInternalPrimitive2DClockSprite.h"
#include "sprite_masks_retail.h"
#include "sprite_masks_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" unsigned __fastcall SpriteMask(void*,void*);
extern "C" unsigned __fastcall ClockMask(void*,void*);
static void require(bool value){if(!value)abort();}
static unsigned __declspec(noinline) sprite(CEngineInternalPrimitive2DSprite* p){return p->GetRenderLayerMask();}
static unsigned __declspec(noinline) clockSprite(CEngineInternalPrimitive2DClockSprite* p){return p->GetRenderLayerMask();}
int main(){
    unsigned char* code=(unsigned char*)VirtualAlloc(0,32,0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
    unsigned storage[1024],before[1024],table[29];unsigned cases=0;
    for(unsigned pattern=0;pattern<256;++pattern)for(unsigned kind=0;kind<2;++kind)for(unsigned location=0;location<2;++location)for(unsigned fill=0;fill<2;++fill){
        unsigned value=pattern==0?0:pattern==1?0xffffffff:pattern==2?0x80000000:pattern*0x13579bdf;
        unsigned char* p=(unsigned char*)storage+(location?256:0);unsigned results[2];
        for(unsigned run=0;run<2;++run){
            memset(storage,fill?0x95:0,sizeof(storage));memset(table,0,sizeof(table));
            table[28]=run?(unsigned)(kind?ClockMask:SpriteMask):(unsigned)(code+(kind?8:0));
            *(void***)p=(void**)table;*(unsigned*)(p+0x30)=value;memcpy(before,storage,sizeof(storage));
            results[run]=kind?clockSprite((CEngineInternalPrimitive2DClockSprite*)p):sprite((CEngineInternalPrimitive2DSprite*)p);
            require(!memcmp(before,storage,sizeof(storage)));require(results[run]==(kind?0x80000000:value));
        }
        require(results[0]==results[1]);++cases;
    }
    VirtualFree(code,0,0x8000);printf("NATIVE_SPRITE_MASKS_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;
}
