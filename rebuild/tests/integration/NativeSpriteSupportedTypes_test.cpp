#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEnginePrimitiveManagerSprites.h"
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEnginePrimitiveTypeList.h"
#include "sprite_supported_retail.h"
#include "sprite_supported_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall SpriteTypes(void*,void*,CEnginePrimitiveTypeList&);
extern "C" void __fastcall TwoDTypes(void*,void*,CEnginePrimitiveTypeList&);
static unsigned char* arena;static unsigned char snapshots[2][6][4096];
static unsigned run,count,allocation,mutation,listOffset,cases,total,originalCount;
static unsigned& word(unsigned off){return *(unsigned*)(arena+off);}
static void snapshot(){if(count==6)abort();memcpy(snapshots[run][count++],arena,4096);++total;}
extern "C" void* Allocate(size_t bytes){
    snapshot();unsigned at=2048+allocation++*512;if(allocation>2||bytes>512)abort();
    if((mutation&1)&&originalCount){unsigned start=word(listOffset);*(unsigned*)start=0x65432101;}
    if(mutation&2)word(0x208)^=0x12345678;
    return arena+at;
}
extern "C" void Free(void* memory){if(memory&&memory!=arena+1024&&memory!=arena+2048)abort();snapshot();}
static void require(bool result){if(!result){printf("SUPPORTED_FAIL cases=%u mutation=%u\n",cases,mutation);fflush(stdout);abort();}}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,4096,0x3000,4);require(arena==(void*)0x24000000);
    unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned at=retailFixups[i][0],target=retailFixups[i][1];void* to=target==0x440990?code+256:target==0xbfea0e?(void*)Allocate:target==0xbfea14?(void*)Free:(void*)memmove;*(unsigned*)(code+at)=(unsigned)to-(unsigned)(code+at+4);}
    typedef void (__fastcall* Method)(void*,void*,CEnginePrimitiveTypeList&);
    for(unsigned old=0;old<=8;++old)for(unsigned spare=0;spare<=3;++spare)for(unsigned kind=0;kind<2;++kind)
    for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place){
        listOffset=place?128:0;originalCount=old;unsigned counts[2];
        for(run=0;run<2;++run){
            memset(arena,0x95,4096);unsigned start=(old+spare)?(unsigned)(arena+1024):0;
            word(listOffset)=start;word(listOffset+4)=start+old*4;word(listOffset+8)=start+(old+spare)*4;
            for(unsigned j=0;j<old;++j)word(1024+j*4)=0x100+j;
            count=allocation=0;Method method=run?(kind?TwoDTypes:SpriteTypes):(Method)(code+(kind?64:0));
            method(arena+512,0,*(CEnginePrimitiveTypeList*)(arena+listOffset));snapshot();counts[run]=count;
            unsigned added=kind?2:1;unsigned* result=(unsigned*)word(listOffset);
            require((word(listOffset+4)-word(listOffset))/4==old+added);
            for(unsigned k=0;k<old;++k)require(result[k]==((allocation&&(mutation&1)&&k==0)?0x65432101:0x100+k));
            require(result[old]==(kind?34:7));if(kind)require(result[old+1]==35);
        }
        require(counts[0]==counts[1]);require(!memcmp(snapshots[0],snapshots[1],counts[0]*4096));++cases;
    }
    VirtualFree(code,0,0x8000);VirtualFree(arena,0,0x8000);
    printf("NATIVE_SPRITE_SUPPORTED_TYPES_PASS cases=%u snapshots=%u\n",cases,total);return 0;
}
