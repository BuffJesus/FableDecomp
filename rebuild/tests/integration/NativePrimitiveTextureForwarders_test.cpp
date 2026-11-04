#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_texture_aliases.h"
#include "primitive_texture_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* PrimitiveGlobal;
extern "C" void* ResourceGlobal;
extern "C" void __fastcall Animation(void*,void*,void*,void*);
extern "C" void __fastcall Position(void*,void*,void*,void*);
extern "C" void* __fastcall Allocate(void*,void*,unsigned,void*,long,void*);
extern "C" void __fastcall Free(void*,void*,void*);
static unsigned char* arena;
static unsigned p[6],events;
static FILE* output;
static void Snapshot(unsigned event,unsigned value) {
    fwrite(&event,4,1,output);fwrite(&value,4,1,output);fwrite(&PrimitiveGlobal,4,1,output);fwrite(&ResourceGlobal,4,1,output);fwrite(arena,1,4096,output);
}
static void Check(void* self,void* expected,unsigned event) {
    if(self!=expected || ++events!=event)abort();
    ++*(unsigned*)(arena+0x40);
}
extern "C" void __fastcall SetAnimation(void* self,void*,void* handle,void* value) {
    Check(self,PrimitiveGlobal,1);
    if(handle!=arena+0x100 || value!=arena+p[5])abort();
    *(unsigned*)handle^=*(unsigned*)value;
    if(p[0]&1)PrimitiveGlobal=arena+0x400;
    Snapshot(1,(unsigned)self);
}
extern "C" void __fastcall SetPosition(void* self,void*,void* handle,void* value) {
    Check(self,PrimitiveGlobal,2);
    if(handle!=arena+0x100 || value!=arena+p[5])abort();
    *(unsigned*)value+=*(unsigned*)handle;
    if(p[0]&2)ResourceGlobal=arena+0x800;
    Snapshot(2,(unsigned)self);
}
extern "C" void* __fastcall AllocateExternal(void* self,void*,unsigned flags,void* size,long levels,void* format) {
    Check(self,ResourceGlobal,3);
    if(flags!=p[2] || size!=arena+0x120 || (unsigned)levels!=p[3] || format!=arena+p[5])abort();
    *(unsigned*)(arena+0x44)=flags;*(long*)(arena+0x48)=levels;
    if(p[0]&4)ResourceGlobal=arena+0xa00;
    Snapshot(3,(unsigned)self);
    return p[4]?arena+p[4]:0;
}
extern "C" void __fastcall FreeExternal(void* self,void*,void* texture) {
    Check(self,ResourceGlobal,4);
    if(texture!=(p[4]?arena+p[4]:0))abort();
    if(texture)++*(unsigned*)texture;
    Snapshot(4,(unsigned)self);
}
int main(int argc,char** argv) {
    if(argc!=4||PrimitiveGlobal||ResourceGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;
    memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i) {
        unsigned address=retailFixups[i][1],value=0;
        switch(address) {
        case 0x1436e80:value=(unsigned)&PrimitiveGlobal;break;
        case 0x1436e98:value=(unsigned)&ResourceGlobal;break;
        case 0xb31e90:value=(unsigned)&SetAnimation;break;
        case 0xb31ec0:value=(unsigned)&SetPosition;break;
        case 0xb3b1f0:value=(unsigned)&AllocateExternal;break;
        case 0xb3b5b0:value=(unsigned)&FreeExternal;break;
        default:abort();
        }
        if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);
        *(unsigned*)(code+retailFixups[i][0])=value;
    }
    typedef void (__fastcall *Setter)(void*,void*,void*,void*);
    typedef void* (__fastcall *Allocator)(void*,void*,unsigned,void*,long,void*);
    typedef void (__fastcall *Freer)(void*,void*,void*);
    bool retail=strcmp(argv[3],"retail")==0;
    Setter animation=retail?(Setter)code:&Animation,position=retail?(Setter)(code+16):&Position;
    Allocator allocate=retail?(Allocator)(code+32):&Allocate;Freer freeTexture=retail?(Freer)(code+48):&Free;
    unsigned cases=0;
    while(fread(p,4,6,in)==6) {
        if(fread(arena,1,4096,in)!=4096)return 6;
        PrimitiveGlobal=arena+(p[1]?0x300:0x200);ResourceGlobal=arena+(p[1]?0x700:0x600);events=0;
        animation((void*)0x1badb002,0,arena+0x100,arena+p[5]);
        position((void*)0x1badb002,0,arena+0x100,arena+p[5]);
        void* result=allocate((void*)0x1badb002,0,p[2],arena+0x120,p[3],arena+p[5]);
        if(result!=(p[4]?arena+p[4]:0))abort();
        freeTexture((void*)0x1badb002,0,result);
        if(events!=4)abort();Snapshot(5,(unsigned)result);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_TEXTURE_FORWARDERS_PASS cases=%u\n",cases);return 0;
}
