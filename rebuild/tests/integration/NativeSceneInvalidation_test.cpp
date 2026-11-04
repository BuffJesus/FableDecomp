#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scene_invalidation_aliases.h"
#include "scene_invalidation_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall InvalidateScene(void*,void*);
static unsigned char* arena;
static unsigned p[2],ids[9],calls;
static FILE* output;
static unsigned& At(unsigned offset){return *(unsigned*)(arena+offset);}
static void Record(unsigned tag,unsigned value){fwrite(&tag,4,1,output);fwrite(&value,4,1,output);fwrite(arena,1,16384,output);}
extern "C" void __fastcall InvalidateGrid(void* self,void*,unsigned x,unsigned y,bool children){
    if(calls>=p[0] || x || y || !children)abort();unsigned id=ids[calls];
    unsigned expected=0x2000+32*id;if(calls && (p[1]==1||p[1]==2))expected=0x3800+32*id;
    if(self!=arena+expected)abort();++At(expected+4);++calls;
    if(calls<p[0]){
        unsigned next=ids[calls];
        if(p[1]==1)At(0x1400+64*next+0x18)=(unsigned)(arena+0x3400+16*next);
        if(p[1]==2)At(0x600+48*next+0x14)=(unsigned)(arena+0x3000+64*next);
        if(p[1]==3)At(0x600+48*next+0x10)^=0xffffffff;
    }
    Record(1,(unsigned)self);
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+47)=(unsigned)&InvalidateGrid-(unsigned)(code+51);
    typedef void (__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&InvalidateScene;unsigned cases=0;
    while(fread(p,4,2,input)==2){
        if(fread(ids,4,9,input)!=9||fread(arena,1,16384,input)!=16384)return 6;calls=0;fn(arena,0);if(calls!=p[0])abort();Record(2,calls);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_SCENE_INVALIDATION_PASS cases=%u\n",cases);return 0;
}
