#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_post_reset_aliases.h"
#include "engine_post_reset_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Reset(void*,void*);
static unsigned char* arena;
static FILE* output;
static unsigned p[4],events,returnValue;
static void Snapshot(unsigned tag,unsigned value) {
    fwrite(&tag,4,1,output);fwrite(&value,4,1,output);fwrite(arena,1,1024,output);
}
extern "C" unsigned __fastcall Restore(void* self,void*) {
    if(self!=arena+p[0] || ++events>2)abort();
    ++*(unsigned*)(arena+0x380);
    if(p[2])arena[p[0]+0xbd]=(unsigned char)(p[2]==1?0:255);
    Snapshot(0x100+events,(unsigned)self);return returnValue;
}
int main(int argc,char** argv) {
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;
    memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+14)=(unsigned)&Restore-(unsigned)(code+18);
    typedef bool (__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Reset;unsigned cases=0;
    while(fread(p,4,4,in)==4) {
        if(fread(arena,1,1024,in)!=1024)return 6;events=0;
        for(unsigned j=0;j<2;++j) {
            returnValue=j?p[1]^3:p[1];unsigned before=events;
            bool enabled=arena[p[0]+0xbd]!=0;
            bool result=fn(arena+p[0]+12,0);
            if(events!=before+(enabled?1:0) || result!=(enabled?(returnValue!=2):true))abort();
            Snapshot(j,result?1:0);
        }
        ++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_ENGINE_POST_RESET_PASS cases=%u\n",cases);return 0;
}
