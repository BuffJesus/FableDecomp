#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "effect_quality_aliases.h"
#include "effect_quality_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* ResourceGlobal;
extern "C" bool __fastcall GlowQuality(void*,void*,void*);
extern "C" bool __fastcall RadialQuality(void*,void*,void*);
extern "C" bool __fastcall DisplacementQuality(void*,void*,void*);
extern "C" void __fastcall GlowRelease(void*,void*);
extern "C" void __fastcall RadialRelease(void*,void*);
extern "C" void __fastcall DisplacementRelease(void*,void*);
static unsigned char* arena;
static unsigned p[6],events;
static FILE* output;
static const unsigned owners[3]={0x400,0x800,0xc00},handles[3]={0x30,0x1c,0x18},supported[3]={0x21c,0x1c8,0xed};
static void Snapshot(unsigned tag,unsigned result) {
    if(++events>12)abort();fwrite(&tag,4,1,output);fwrite(&result,4,1,output);fwrite(&ResourceGlobal,4,1,output);fwrite(arena,1,20480,output);
}
static bool Init(void* self,unsigned kind) {
    if(kind!=p[0] || self!=arena+owners[kind])abort();
    if(p[5]==1)*(long*)((unsigned char*)self+handles[kind])=-1;
    if(p[5]==2) {*(long*)((unsigned char*)self+handles[kind])=17;ResourceGlobal=arena+0x1800;}
    ++*(unsigned*)(arena+0x40);Snapshot(1,p[3]);return p[3]!=0;
}
extern "C" bool __fastcall InitGlow(void* self,void*) {return Init(self,0);}
extern "C" bool __fastcall InitRadial(void* self,void*) {return Init(self,1);}
extern "C" bool __fastcall InitDisplacement(void* self,void*) {return Init(self,2);}
extern "C" void __fastcall FreePage(void* self,void*,long handle) {
    if(self!=(unsigned char*)ResourceGlobal+0x3190 || handle<0 || handle!=*(long*)(arena+owners[p[0]]+handles[p[0]]))abort();
    if(p[5]==3) {arena[owners[p[0]]+8]=77;*(long*)(arena+owners[p[0]]+handles[p[0]])=29;}
    ++*(unsigned*)(arena+0x44);Snapshot(2,handle);
}
int main(int argc,char** argv) {
    if(argc!=4||ResourceGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i) {
        unsigned value=0;
        switch(retailFixups[i][1]) {
        case 0x1436e98:value=(unsigned)&ResourceGlobal;break;
        case 0xf37030:value=(unsigned)&FreePage;break;
        case 0xb5ece0:value=(unsigned)&InitGlow;break;
        case 0xb5bf10:value=(unsigned)&InitRadial;break;
        case 0xb86a70:value=(unsigned)&InitDisplacement;break;
        default:abort();
        }
        if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);
        *(unsigned*)(code+retailFixups[i][0])=value;
    }
    typedef bool (__fastcall *Quality)(void*,void*,void*);typedef void (__fastcall *Release)(void*,void*);
    bool retail=strcmp(argv[3],"retail")==0;
    Quality quality[3]={retail?(Quality)code:&GlowQuality,retail?(Quality)(code+160):&RadialQuality,retail?(Quality)(code+320):&DisplacementQuality};
    Release release[3]={retail?(Release)(code+128):&GlowRelease,retail?(Release)(code+288):&RadialRelease,retail?(Release)(code+496):&DisplacementRelease};unsigned cases=0;
    while(fread(p,4,6,in)==6) {
        if(fread(arena,1,20480,in)!=20480)return 6;events=0;ResourceGlobal=arena+0x1000;
        unsigned kind=p[0];bool requested=(kind==0?(p[1]&4):kind==1?(p[1]&8):(p[1]&3))!=0;
        bool expected=!requested || !p[2] || p[3];
        for(unsigned j=0;j<2;++j) {
            bool result=quality[kind](arena+owners[kind],0,arena+0x100);
            if(result!=expected)abort();Snapshot(3+j,result?1:0);
            if(!j) {release[kind](arena+owners[kind],0);if(*(long*)(arena+owners[kind]+handles[kind])!=-1)abort();Snapshot(5,kind);}
        }
        ++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_EFFECT_QUALITY_PASS cases=%u\n",cases);return 0;
}
