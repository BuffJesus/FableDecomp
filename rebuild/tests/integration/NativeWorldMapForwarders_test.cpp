#include <stdio.h>
#include <string.h>
#include "world_map_forwarders_aliases.h"
#include "world_map_forwarders_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* MapGlobal;
extern "C" void __fastcall Entry0(void*);
extern "C" void __fastcall Entry1(void*,void*,unsigned);
extern "C" void __fastcall Entry2(void*,void*,unsigned);
extern "C" void __fastcall Entry3(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Entry4(void*,void*,unsigned);
extern "C" void __fastcall Entry5(void*,void*,unsigned);
extern "C" void __fastcall Entry6(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Entry7(void*,void*,unsigned);
extern "C" void __fastcall Entry8(void*,void*,unsigned);
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self,unsigned a=0,unsigned b=0){
    unsigned h[]={kind,(unsigned)self,a,b,(unsigned)MapGlobal};fwrite(h,4,5,output);fwrite(arena,1,4096,output);
    if(mode&&kind<9){memset(self,0x91+kind,16);MapGlobal=arena+((self==arena)?0x800:0);}
}
extern "C" void __fastcall Boundary0(void* self){record(0,self);}
extern "C" void __fastcall Boundary1(void* self,void*,unsigned a){record(1,self,a);}
extern "C" void __fastcall Boundary2(void* self,void*,unsigned a){record(2,self,a);}
extern "C" void __fastcall Boundary3(void* self,void*,unsigned a,unsigned b){record(3,self,a,b);}
extern "C" void __fastcall Boundary4(void* self,void*,unsigned a){record(4,self,a);}
extern "C" void __fastcall Boundary5(void* self,void*,unsigned a){record(5,self,a);}
extern "C" void __fastcall Boundary6(void* self,void*,unsigned a,unsigned b){record(6,self,a,b);}
extern "C" void __fastcall Boundary7(void* self,void*,unsigned a){record(7,self,a);}
extern "C" void __fastcall Boundary8(void* self,void*,unsigned a){record(8,self,a);}
int main(int argc,char** argv){
    if(argc!=4||MapGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    void* boundaries[]={(void*)Boundary0,(void*)Boundary1,(void*)Boundary2,(void*)Boundary3,(void*)Boundary4,(void*)Boundary5,(void*)Boundary6,(void*)Boundary7,(void*)Boundary8};
    void* entries[]={(void*)Entry0,(void*)Entry1,(void*)Entry2,(void*)Entry3,(void*)Entry4,(void*)Entry5,(void*)Entry6,(void*)Entry7,(void*)Entry8};
    for(unsigned i=0;i<9;++i){*(unsigned*)(code+16*i+2)=(unsigned)&MapGlobal;*(unsigned*)(code+16*i+7)=(unsigned)boundaries[i]-(unsigned)(code+16*i+11);}
    typedef void (__fastcall *NoArgs)(void*);typedef void (__fastcall *OneArg)(void*,void*,unsigned);typedef void (__fastcall *TwoArgs)(void*,void*,unsigned,unsigned);
    unsigned p[5],cases=0;
    while(fread(p,4,5,in)==5){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[1];MapGlobal=arena+p[2];void* entry=argv[3][0]=='r'?(void*)(code+16*p[0]):entries[p[0]];
        if(p[0]==0)((NoArgs)entry)((void*)0x1badb002);
        else if(p[0]==3||p[0]==6)((TwoArgs)entry)((void*)0x1badb002,0,p[3],p[4]);
        else ((OneArg)entry)((void*)0x1badb002,0,p[3]);
        record(9,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_WORLD_MAP_FORWARDERS_PASS cases=%u\n",cases);return 0;
}
