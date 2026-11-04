#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scratch_release_aliases.h"
#include "scratch_release_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Page(void*,void*,long);
extern "C" void __fastcall Entry(void*,void*,long);
static unsigned char* arena;
static FILE* output;
static unsigned p[5],events,allocations,deletions,surfaceCalls;
static void Snapshot(unsigned tag,unsigned value) {
    if(++events>64)abort();fwrite(&tag,4,1,output);fwrite(&value,4,1,output);fwrite(arena,1,8192,output);
}
extern "C" void* __cdecl Allocate(unsigned size) {
    if(size>128 || allocations>=8)abort();void* result=arena+0x1000+128*allocations++;
    if(p[3]==3)*(void**)(arena+4)=arena+0x600;
    Snapshot(1,size);return result;
}
extern "C" void __cdecl ArenaFree(void* pointer) {
    unsigned offset=(unsigned char*)pointer-arena;if(offset>=8192)abort();Snapshot(2,offset);
}
static void __cdecl DeleteObject(void* pointer) {
    if(!arena || (unsigned char*)pointer<arena || (unsigned char*)pointer>=arena+8192) {free(pointer);return;}
    unsigned offset=(unsigned char*)pointer-arena;
    if(offset<0xb00 || offset>=0xb30 || (offset-0xb00)%16)abort();++deletions;
    if(p[3]==1 && deletions==1)*(void**)(arena+4)=arena+0x600;
    if(p[3]==5) {
        unsigned char* resources=*(unsigned char**)(arena+4);unsigned index=(offset-0xb00)/16;
        *(void**)(resources+index*44+8)=arena+0xc80+32*index;
    }
    Snapshot(3,offset);
}
void __cdecl operator delete(void* pointer) {DeleteObject(pointer);}
static unsigned __stdcall ReleaseTexture(void* self) {
    unsigned offset=(unsigned char*)self-arena;if(offset<0xb80 || offset>=0xbb0 || (offset-0xb80)%16)abort();
    if(p[3]==4)*(unsigned*)(arena+0xb04+(offset-0xb80))^=0xa0000000;
    Snapshot(4,offset);return 0;
}
static void* __fastcall DeleteSurface(void* self,void*,unsigned flags) {
    unsigned offset=(unsigned char*)self-arena;if(flags!=1 || offset<0xc00 || offset>=0xce0 || (offset-0xc00)%32)abort();++surfaceCalls;
    if(p[3]==2 && surfaceCalls==1)*(void**)(arena+0x10)=arena+0x300;
    Snapshot(5,offset);return self;
}
int main(int argc,char** argv) {
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i) {
        unsigned value=0;
        switch(retailFixups[i][1]) {
        case 0xf36f40:value=(unsigned)(code+160);break;
        case 0x440990:value=(unsigned)(code+416);break;
        case 0x9f9f70:value=(unsigned)(code+640);break;
        case 0xbfe9bc:value=(unsigned)&DeleteObject;break;
        case 0xbfea0e:value=(unsigned)&Allocate;break;
        case 0xbfea14:value=(unsigned)&ArenaFree;break;
        case 0xbfeae6:value=(unsigned)&memmove;break;
        default:abort();
        }
        *(unsigned*)(code+retailFixups[i][0])=value-(unsigned)(code+retailFixups[i][0]+4);
    }
    static void* textureTable[3];textureTable[2]=(void*)&ReleaseTexture;
    static void* surfaceTable[1]={(void*)&DeleteSurface};
    typedef void (__fastcall *Fn)(void*,void*,long);bool retail=strcmp(argv[3],"retail")==0;Fn page=retail?(Fn)code:&Page,entry=retail?(Fn)(code+160):&Entry;unsigned cases=0;
    while(fread(p,4,5,in)==5) {
        if(fread(arena,1,8192,in)!=8192)return 6;events=allocations=deletions=surfaceCalls=0;
        for(unsigned j=0;j<3;++j) {
            *(void***)(arena+0xb80+16*j)=textureTable;
            *(void***)(arena+0xc00+32*j)=surfaceTable;
            *(void***)(arena+0xc80+32*j)=surfaceTable;
        }
        if(p[0])entry(arena,0,p[4]);else page(arena,0,p[1]);
        if(!p[0]) {
            unsigned char* pages=*(unsigned char**)(arena+0x10);
            if(*(unsigned*)(pages+p[1]*28+4)!=*(unsigned*)(pages+p[1]*28+8))abort();
        }
        Snapshot(6,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_SCRATCH_RELEASE_PASS cases=%u\n",cases);return 0;
}
