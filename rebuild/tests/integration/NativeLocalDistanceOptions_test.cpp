#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "local_distance_options_aliases.h"
#include "local_distance_options_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall UpdateOptions(void*,void*,void*);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    typedef void (__fastcall *Fn)(void*,void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&UpdateOptions;unsigned p[3],cases=0;unsigned char expected[2048];
    while(fread(p,4,3,input)==3){
        if(fread(arena,1,2048,input)!=2048||fread(expected,1,2048,input)!=2048)return 6;
        _controlfp(_MCW_EM|p[2],_MCW_EM|_MCW_PC|_MCW_RC);_clearfp();fn(arena+p[0],0,arena+p[1]);unsigned status=_statusfp();
        if(memcmp(arena,expected,2048))abort();fwrite(&status,4,1,output);fwrite(arena,1,2048,output);++cases;
    }
    _clearfp();fclose(input);fclose(output);printf("NATIVE_LOCAL_DISTANCE_OPTIONS_PASS cases=%u\n",cases);return 0;
}
