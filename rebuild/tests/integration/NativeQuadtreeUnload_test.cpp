#include <stdio.h>
#include <string.h>
#include "quadtree_unload_aliases.h"
#include "quadtree_unload_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,void*);
extern "C" void __fastcall DetailEntry(void*,void*,bool);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode,destroyed;
static void record(unsigned kind,void* self,unsigned argument){
    unsigned h[]={kind,(unsigned)self,argument,destroyed};fwrite(h,4,4,output);
    fwrite(arena,1,4096,output);
}
extern "C" void __fastcall Release(void* self,void*,long id){
    record(1,self,(unsigned)id);if(self)memset(self,0x91,32);
    if(mode==1)owner[0x48]|=4;
    if(mode==2)owner[0x48]&=~4;
}
extern "C" void __fastcall Remove(void* self){
    record(2,self,0);owner[0x48]^=0x80;
    if(mode==3)*(void**)(owner+12)=0;
    if(mode==4)*(void**)(owner+12)=arena+0x700;
}
extern "C" void __fastcall Finish(void* self){record(5,self,0);memset(self,0x95,32);}
extern "C" void* __fastcall Destroy(void* self,void*,unsigned flags){
    record(3,self,flags);++destroyed;memset(self,0xdd,0x4c);
    if(mode==5){*(void**)(owner+20)=0;owner[0x48]|=8;}
    return self;
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    // Fixed dispatch table keeps full-state snapshots byte-for-byte comparable.
    unsigned* table=(unsigned*)VirtualAlloc((void*)0x25000000,4096,0x3000,4);if(table!=(void*)0x25000000)return 7;table[0]=(unsigned)Destroy;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+18)=(unsigned)Release-(unsigned)(code+22);*(unsigned*)(code+37)=(unsigned)Remove-(unsigned)(code+41);
    *(unsigned*)(code+128+19)=(unsigned)code-(unsigned)(code+128+23);
    *(unsigned*)(code+128+31)=(unsigned)Finish-(unsigned)(code+128+35);
    typedef void (__fastcall *Call)(void*,void*,void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    typedef void (__fastcall *DetailCall)(void*,void*,bool);DetailCall detail=argv[3][0]=='r'?(DetailCall)(code+128):DetailEntry;
    unsigned p[4],cases=0;
    while(fread(p,4,4,in)==4){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];owner=arena+p[1];destroyed=0;
        for(unsigned i=0;i<2;++i){
            if(p[3])detail(owner-12,0,p[3]==2);
            else entry(owner,0,p[2]?(void*)(arena+0x900):0);
            record(4,0,i);
        }
        ++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_QUADTREE_UNLOAD_PASS cases=%u\n",cases);return 0;
}
