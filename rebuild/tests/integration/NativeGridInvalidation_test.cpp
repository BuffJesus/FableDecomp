#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid_invalidation_aliases.h"
#include "grid_invalidation_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall InvalidateGrid(void*,void*,unsigned,unsigned,bool);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+91)=(unsigned)code-(unsigned)(code+95);
    typedef void (__fastcall *Fn)(void*,void*,unsigned,unsigned,bool);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&InvalidateGrid;
    unsigned p[3],cases=0;unsigned char expected[16384];
    while(fread(p,4,3,input)==3){
        if(fread(arena,1,16384,input)!=16384 || fread(expected,1,16384,input)!=16384)return 6;
        fn(arena+0x100,0,p[0],p[1],p[2]!=0);if(memcmp(arena,expected,16384))abort();fwrite(arena,1,16384,output);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_GRID_INVALIDATION_PASS cases=%u\n",cases);return 0;
}
