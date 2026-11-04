#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_pre_reset_aliases.h"
#include "engine_pre_reset_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall PreReset(void*,void*);
static unsigned char* arena;
static unsigned mode,events,deleted,released;
static FILE* output;
static void Snapshot(unsigned tag,unsigned receiver) {
    if(++events>24)abort();fwrite(&tag,4,1,output);fwrite(&receiver,4,1,output);fwrite(arena,1,4096,output);
}
extern "C" unsigned __fastcall RestoreTrap(void*,void*) { abort();return 0; }
static void* __fastcall DeleteSurface(void* self,void*,unsigned flags) {
    unsigned offset=(unsigned char*)self-arena;
    if(flags!=1 || offset<0x900 || offset>=0xc00 || (offset-0x900)%64)abort();
    ++deleted;++*(unsigned*)((unsigned char*)self+4);
    if(deleted==1) {
        if(mode==1)*(void**)(arena+0x150)=*(void**)(arena+0x14c);
        if(mode==2) {*(void**)(arena+0x14c)=arena+0x600;*(void**)(arena+0x150)=arena+0x60c;}
        if(mode==5) {*(void**)(arena+0x168)=arena+0x800;*(void**)(arena+0x16c)=arena+0x808;}
    }
    Snapshot(1,offset);return self;
}
static void __fastcall ReleaseComponent(void* self,void*) {
    unsigned offset=(unsigned char*)self-arena;
    if(offset<0xc00 || offset>=0xd00 || (offset-0xc00)%32)abort();
    ++released;++*(unsigned*)((unsigned char*)self+4);
    if(released==1) {
        if(mode==3)*(void**)(arena+0x16c)=(unsigned char*)*(void**)(arena+0x168)+4;
        if(mode==4)*(void**)(arena+0x16c)=(unsigned char*)*(void**)(arena+0x168)+16;
    }
    Snapshot(2,offset);
}
int main(int argc,char** argv) {
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;
    memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+99)=(unsigned)&memmove-(unsigned)(code+103);
    static void* surfaceTable[1]={(void*)&DeleteSurface};static void* componentTable[13];componentTable[11]=(void*)&ReleaseComponent;
    typedef void (__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&PreReset;unsigned cases=0;
    while(fread(&mode,4,1,in)==1) {
        if(fread(arena,1,4096,in)!=4096)return 6;events=deleted=released=0;
        for(unsigned i=0;i<12;++i)*(void***)(arena+0x900+i*64)=surfaceTable;
        for(unsigned i=0;i<8;++i)*(void***)(arena+0xc00+i*32)=componentTable;
        fn(arena+12,0);
        if(*(void**)(arena+0x14c)!=*(void**)(arena+0x150))abort();
        Snapshot(3,(deleted<<16)|released);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_ENGINE_PRE_RESET_PASS cases=%u\n",cases);return 0;
}
