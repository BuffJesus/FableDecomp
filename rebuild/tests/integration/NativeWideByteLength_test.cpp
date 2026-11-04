#include <stdio.h>
#include <string.h>
#include "wide_length_aliases.h"
#include "wide_length_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned __fastcall ByteLength(void*);
int main(int argc,char** argv)
{
    if(argc!=4)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
    typedef unsigned (__fastcall *Fn)(void*);Fn fn=argv[3][0]=='r'?(Fn)code:ByteLength;
    unsigned p[2],cases=0;unsigned char before[1024];
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,1024,in)!=1024)return 5;memcpy(before,arena,1024);
        unsigned result=fn(arena+p[0]);
        if(result!=p[1]||memcmp(before,arena,1024))return 6;
        if(fwrite(&result,4,1,out)!=1||fwrite(arena,1,1024,out)!=1024)return 7;++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_WIDE_BYTE_LENGTH_PASS cases=%u\n",cases);return 0;
}
