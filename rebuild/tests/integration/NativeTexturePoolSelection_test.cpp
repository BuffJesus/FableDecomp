#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_pool_selection_aliases.h"
#include "texture_pool_selection_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall ResizeManager(void*,void*,unsigned,unsigned);
extern "C" long __fastcall GetIndex(void*,void*,unsigned);
static unsigned char* arena;
static unsigned p[6],frames,resizes;
static long selected;
static FILE* output;
static void Record(unsigned tag,unsigned a=0,unsigned b=0){unsigned words[3]={tag,a,b};fwrite(words,4,3,output);fwrite(arena,1,8192,output);}
extern "C" void __fastcall Frame(void* self,void*){
    unsigned char* manager=arena+p[0];if(self!=manager||++frames!=1||selected<0)abort();
    if(p[4]==1)*(unsigned*)(manager+8)=0;
    if(p[4]==2)*(unsigned*)(manager+0xc+4*selected)^=0xffffffff;
    if(p[4]==3)*(unsigned*)(manager+8)=16;
    if(p[4]==4)memset(manager+0x58c,0,64);
    Record(2,(unsigned)self,selected);
}
extern "C" bool __fastcall Resize(void* self,void*,unsigned count){
    if(self!=arena+p[0]+0x4c+0x54*selected||count!=p[3]||++resizes!=1)abort();
    *(unsigned*)((unsigned char*)self+0x3c)=count;Record(3,(unsigned)self,count);return p[5]!=0;
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<3;++i){unsigned address=retailFixups[i][1];unsigned target=address==0xa69730?(unsigned)(code+64):address==0xa698b0?(unsigned)&Frame:(unsigned)&Resize;*(unsigned*)(code+retailFixups[i][0])=target-(unsigned)(code+retailFixups[i][0]+4);}
    typedef bool (__fastcall *ResizeFn)(void*,void*,unsigned,unsigned);typedef long (__fastcall *IndexFn)(void*,void*,unsigned);bool retail=strcmp(argv[3],"retail")==0;ResizeFn resize=retail?(ResizeFn)code:&ResizeManager;IndexFn index=retail?(IndexFn)(code+64):&GetIndex;unsigned cases=0;unsigned char before[8192];
    while(fread(p,4,6,input)==6){
        if(fread(arena,1,8192,input)!=8192)return 6;frames=resizes=0;memcpy(before,arena,8192);unsigned char* manager=arena+p[0];unsigned* sizes=(unsigned*)(manager+0xc);
        selected=index(manager,0,p[1]);if(memcmp(before,arena,8192))abort();
        if(selected<0){if(selected!=-1||(p[2]&&p[1]<=sizes[p[2]-1]))abort();}
        else {if((unsigned)selected>=p[2]||sizes[selected]<p[1])abort();if(sizes[selected]!=p[1]&&selected&&sizes[selected-1]>=p[1])abort();}
        Record(1,selected);bool result=resize(manager,0,p[1],p[3]);
        if(result!=(selected>=0&&p[5]!=0)||frames!=(selected>=0?1:0)||resizes!=frames)abort();Record(4,result);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_TEXTURE_POOL_SELECTION_PASS cases=%u\n",cases);return 0;
}
