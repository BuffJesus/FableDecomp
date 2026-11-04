#include <stdio.h>
#include <string.h>
#include "block_release_aliases.h"
#include "block_release_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ReleaseEntry(void*,void*,long);
extern "C" void* __fastcall FindEntry(void*,void*,void*,const long*);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+15)=(unsigned)(code+64)-(unsigned)(code+19);
    typedef void (__fastcall *Release)(void*,void*,long);typedef void* (__fastcall *Find)(void*,void*,void*,const long*);
    Release release=argv[3][0]=='r'?(Release)code:ReleaseEntry;Find find=argv[3][0]=='r'?(Find)(code+64):FindEntry;
    unsigned p[4],cases=0;
    while(fread(p,4,4,in)==4){
        if(fread(arena,1,4096,in)!=4096)return 5;unsigned result=0;
        if(p[0])release(arena+p[1],0,*(long*)(arena+p[2]));
        else result=(unsigned)find(arena+p[1]+8,0,arena+p[3],(long*)(arena+p[2]));
        fwrite(&result,4,1,out);fwrite(arena,1,4096,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_BLOCK_RELEASE_PASS cases=%u\n",cases);return 0;
}
