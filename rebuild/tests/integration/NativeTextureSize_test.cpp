#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_size_aliases.h"
#include "texture_size_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned __fastcall TextureSize(unsigned,unsigned,bool,void*);
extern "C" const unsigned char FormatTable[1692];
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned address=retailFixups[i][1];unsigned value=address==0x9e3820?(unsigned)(code+144):(unsigned)FormatTable+address-0x129ba40;if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}
    typedef unsigned (__fastcall *Fn)(unsigned,unsigned,bool,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&TextureSize;unsigned cases=0,p[6];unsigned char original[64],table[1692];memcpy(table,FormatTable,1692);
    while(fread(p,4,6,input)==6){
        if(fread(arena,1,64,input)!=64)return 6;memcpy(original,arena,64);
        unsigned result=fn(p[0],p[1],p[2]!=0,arena+p[5]);if(result!=p[4]||memcmp(arena,original,64)||memcmp(table,FormatTable,1692))abort();fwrite(&result,4,1,output);fwrite(arena,1,64,output);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_TEXTURE_SIZE_PASS cases=%u\n",cases);return 0;
}
