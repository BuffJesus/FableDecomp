#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank_discard_aliases.h"
#include "bank_discard_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,void*);
extern "C" void __fastcall StaticEntry(void*);
static unsigned char* arena;
static unsigned char* slot;
static FILE* output;
static unsigned mode,abortResult,depth,allocations;
static void record(unsigned kind,void* self,unsigned argument){unsigned h[]={kind,(unsigned)self,argument,depth};fwrite(h,4,4,output);fwrite(arena,1,8192,output);}
static void enter(void* section,unsigned tryOnly){
    record(1,section,tryOnly);++depth;(*(unsigned*)section)++;
    if(mode==1&&depth==1)*(void**)slot=arena+0x1100;
    if(mode==2&&depth==2){arena[0x1044]=1;*(void**)slot=arena+0x1100;}
}
extern "C" void __stdcall EnterProbe(void* section){enter(section,0);}
extern "C" int __stdcall TryProbe(void* section){enter(section,1);return 1;}
extern "C" void __stdcall LeaveProbe(void* section){
    record(2,section,1);(*(unsigned*)section)--;--depth;
    if(mode==3&&depth==1){*(void**)slot=arena+0x1100;*(void**)(slot+4)=arena+0x1940;}
}
extern "C" void (__stdcall *EnterIAT)(void*)=EnterProbe;
extern "C" int (__stdcall *TryIAT)(void*)=TryProbe;
extern "C" void (__stdcall *LeaveIAT)(void*)=LeaveProbe;
extern "C" bool __fastcall Abort(void* self){record(3,self,abortResult);return abortResult!=0;}
extern "C" void* __cdecl PushMalloc(unsigned bytes){
    record(4,0,bytes);if(bytes!=16||allocations++)exit(10);
    void* result=arena+0x1c00;memset(result,0x95,16);return result;
}
extern "C" void __cdecl DisposalFree(void* self){record(5,self,0);memset(self,0xde,16);}
extern "C" void __fastcall DeleteData(void* self){
    record(6,self,0);memset(self,0x93,32);
    if(mode==2)*(void**)(slot+4)=arena+0x1940;
}
void operator delete(void* p){
    if(arena&&(unsigned char*)p>=arena&&(unsigned char*)p<arena+8192){record(7,p,0);memset(p,0xdd,12);}else free(p);
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned at=retailFixups[i][0],target=retailFixups[i][1];void* destination=0;
        switch(target){case 0xa62920:destination=code+768;break;case 0xa629a0:destination=code+832;break;case 0x9d6350:destination=(void*)Abort;break;case 0x9d7b00:destination=code+320;break;case 0xbfea0e:destination=(void*)PushMalloc;break;case 0xbfea14:destination=(void*)DisposalFree;break;case 0x9d6730:destination=code+512;break;case 0xbfe9bc:destination=(void*)(void(*)(void*))operator delete;break;case 0x9d67e0:destination=code;break;case 0x143fe0c:destination=&EnterIAT;break;case 0x143fd30:destination=&TryIAT;break;case 0x143fe10:destination=&LeaveIAT;break;default:return 9;}
        *(unsigned*)(code+at)=(unsigned)destination-(retailFixups[i][2]?(unsigned)(code+at+4):0);
    }
    typedef void (__fastcall *Call)(void*,void*,void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    typedef void (__fastcall *StaticCall)(void*);StaticCall staticEntry=argv[3][0]=='r'?(StaticCall)(code+256):StaticEntry;
    unsigned p[4],cases=0;
    while(fread(p,4,4,in)==4){
        if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];abortResult=p[1];slot=arena+0x1800;depth=0;allocations=0;
        *(unsigned*)(arena+0x1904)=(unsigned)DeleteData;*(unsigned*)(arena+0x1944)=(unsigned)DeleteData;
        *(unsigned*)(arena+0x19a4)=(unsigned)DeleteData;*(unsigned*)(arena+0x19c4)=(unsigned)DeleteData;
        if(p[3])staticEntry(slot);else entry(arena+p[2],0,slot);
        if(depth)return 8;record(8,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_BANK_DISCARD_PASS cases=%u\n",cases);return 0;
}
