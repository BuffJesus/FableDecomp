#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device_restore_aliases.h"
#include "device_restore_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned __fastcall SmartRestore(void*,void*);
extern "C" bool __fastcall PostReset(void*,void*);
extern "C" void* ShadowGlobal;
extern "C" void* DisplacementGlobal;
extern "C" void* GlowGlobal;
extern "C" void* RadialGlobal;
static unsigned char* arena;
static unsigned p[9],events,releaseCalls,restoreCalls;
static FILE* output;
static void Snapshot(unsigned tag,unsigned value) {
    if(++events>160)abort();
    fwrite(&tag,4,1,output);fwrite(&value,4,1,output);
    fwrite(&ShadowGlobal,4,1,output);fwrite(&DisplacementGlobal,4,1,output);fwrite(&GlowGlobal,4,1,output);fwrite(&RadialGlobal,4,1,output);
    fwrite(arena,1,4096,output);
}
static unsigned ComponentIndex(void* self) {
    unsigned offset=(unsigned char*)self-arena;
    if(offset<0x300 || offset>=0x360 || (offset-0x300)%32)abort();return (offset-0x300)/32;
}
static void __fastcall Release(void* self,void*) {
    unsigned index=ComponentIndex(self);++releaseCalls;++*(unsigned*)((unsigned char*)self+4);
    if(p[4]==3 && releaseCalls==1)*(void**)(arena+0x168)=arena+0x204;
    Snapshot(1,index);
}
static bool __fastcall Restore(void* self,void*) {
    unsigned index=ComponentIndex(self);++restoreCalls;++*(unsigned*)((unsigned char*)self+8);
    if(p[4]==4 && restoreCalls==1) {*(unsigned*)(arena+0xe8)=0;*(unsigned*)(arena+0xec)=0;}
    bool result=!(p[3]==6 || (p[3]==7 && index==1));Snapshot(2,(index<<8)|result);return result;
}
extern "C" bool __fastcall Shadow(void* self,void*,long size,float scale) {
    if(self!=ShadowGlobal || size<1000 || size>1003)abort();unsigned quality=size-1000;
    unsigned bits=*(unsigned*)&scale;if(bits!=0x3f800000+quality)abort();
    if(p[4]==1) {GlowGlobal=0;RadialGlobal=0;}
    bool result=!(p[3]==5 || (p[3]==1 && quality>1));Snapshot(3,(quality<<8)|result);return result;
}
static bool __fastcall Effect(void* self,void*,void* definition) {
    unsigned offset=(unsigned char*)self-arena;
    unsigned which=offset==0x800?0:offset==0x840?1:(offset==0x880||offset==0x8a0)?2:99;
    if(which==99 || self!=(which==0?DisplacementGlobal:which==1?GlowGlobal:RadialGlobal))abort();
    unsigned delta=(unsigned char*)definition-(arena+0x400);if(delta%128 || delta/128>3)abort();unsigned quality=delta/128;
    if(p[4]==2 && which==0 && RadialGlobal)RadialGlobal=arena+0x8a0;
    bool result=!(p[3]==5 || (p[3]==2 && which==0 && quality>0) || (p[3]==3 && which==1 && quality>1) || (p[3]==4 && which==2 && quality>0));
    Snapshot(4+which,(quality<<8)|result);return result;
}
int main(int argc,char** argv) {
    if(argc!=4||ShadowGlobal||DisplacementGlobal||GlowGlobal||RadialGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;
    memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i) {
        unsigned value=0;
        switch(retailFixups[i][1]) {
        case 0x1436e60:value=(unsigned)&ShadowGlobal;break;
        case 0x1436e38:value=(unsigned)&DisplacementGlobal;break;
        case 0x1436e34:value=(unsigned)&GlowGlobal;break;
        case 0x1436e30:value=(unsigned)&RadialGlobal;break;
        case 0xb4fc10:value=(unsigned)&Shadow;break;
        case 0xb24500:value=(unsigned)code;break;
        default:abort();
        }
        if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);
        *(unsigned*)(code+retailFixups[i][0])=value;
    }
    static void* componentTable[13];componentTable[11]=(void*)&Release;componentTable[12]=(void*)&Restore;
    static void* effectTable[14];effectTable[13]=(void*)&Effect;
    typedef unsigned (__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&SmartRestore;unsigned cases=0;
    while(fread(p,4,9,in)==9) {
        if(fread(arena,1,4096,in)!=4096)return 6;events=releaseCalls=restoreCalls=0;
        for(unsigned j=0;j<3;++j)*(void***)(arena+0x300+j*32)=componentTable;
        for(unsigned j=0;j<3;++j)*(void***)(arena+0x800+j*64)=effectTable;
        *(void***)(arena+0x8a0)=effectTable;
        ShadowGlobal=(p[2]&1)?arena+0x8c0:0;DisplacementGlobal=(p[2]&2)?arena+0x800:0;GlowGlobal=(p[2]&4)?arena+0x840:0;RadialGlobal=(p[2]&8)?arena+0x880:0;
        unsigned result,expected=p[6];
        if(p[7]) {
            arena[0xbd]=(unsigned char)p[8];
            typedef bool (__fastcall *PostFn)(void*,void*);
            PostFn post=strcmp(argv[3],"retail")==0?(PostFn)(code+416):&PostReset;
            result=post(arena+12,0)?1:0;expected=p[8]?(p[6]!=2):1;
        } else result=fn(arena,0);
        if(result!=expected)abort();Snapshot(7,result);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_DEVICE_RESTORE_PASS cases=%u\n",cases);return 0;
}
