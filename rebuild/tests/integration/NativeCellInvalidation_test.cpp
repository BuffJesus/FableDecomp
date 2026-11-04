#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cell_invalidation_aliases.h"
#include "cell_invalidation_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall InvalidateCell(void*,void*);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    typedef void (__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&InvalidateCell;
    unsigned p[1],cases=0;unsigned char expected[4096];
    while(fread(p,4,1,input)==1){
        if(fread(arena,1,4096,input)!=4096 || fread(expected,1,4096,input)!=4096)return 6;
        fn(arena+p[0],0);if(memcmp(arena,expected,4096))abort();fwrite(arena,1,4096,output);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_CELL_INVALIDATION_PASS cases=%u\n",cases);return 0;
}
