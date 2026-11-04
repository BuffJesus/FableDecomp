#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather_colour_quality_aliases.h"
#include "weather_colour_quality_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Weather(void*,void*,void*);
extern "C" void __fastcall Colour(void*,void*,void*);
int main(int argc,char** argv) {
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    typedef void (__fastcall *Fn)(void*,void*,void*);bool retail=strcmp(argv[3],"retail")==0;Fn functions[2]={retail?(Fn)code:&Weather,retail?(Fn)(code+32):&Colour};
    unsigned offsets[2],cases=0;unsigned char expected[16384];
    while(fread(offsets,4,2,input)==2) {
        if(fread(arena,1,16384,input)!=16384)return 6;
        for(unsigned j=0;j<2;++j) {
            memcpy(expected,arena,16384);
            if(j==0) {expected[0x1008]=expected[offsets[0]+0x71];memcpy(expected+0x22f8,expected+offsets[0]+0x74,4);}
            else expected[0x2808]=(expected[offsets[1]+0x72]!=0 && expected[0x2844]!=0)?1:0;
            functions[j](arena+(j?0x2800:0x1000),0,arena+offsets[j]);
            if(memcmp(arena,expected,16384))abort();fwrite(arena,1,16384,output);
        }
        ++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_WEATHER_COLOUR_QUALITY_PASS cases=%u\n",cases);return 0;
}
