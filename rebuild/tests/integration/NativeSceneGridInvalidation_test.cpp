#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scene_grid_invalidation_aliases.h"
#include "scene_grid_invalidation_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall InvalidateScene(void*,void*);
static unsigned char* arena;
static unsigned p[2],ids[9],calls;
static FILE* output;
static unsigned& At(unsigned offset){return *(unsigned*)(arena+offset);}
static void Record(unsigned tag,unsigned value){fwrite(&tag,4,1,output);fwrite(&value,4,1,output);fwrite(arena,1,32768,output);}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+47)=(unsigned)(code+192)-(unsigned)(code+51);*(unsigned*)(code+283)=(unsigned)(code+192)-(unsigned)(code+287);
    typedef void (__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&InvalidateScene;unsigned cases=0;unsigned char expected[32768];
    while(fread(p,4,2,input)==2){
        if(fread(ids,4,9,input)!=9||fread(arena,1,32768,input)!=32768||fread(expected,1,32768,input)!=32768)return 6;calls=0;fn(arena,0);if(memcmp(arena,expected,32768))abort();Record(2,calls);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_SCENE_GRID_INVALIDATION_PASS cases=%u\n",cases);return 0;
}
