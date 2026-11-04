#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unused_blocks_aliases.h"
#include "unused_blocks_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*);
extern "C" void __fastcall StaticDiscard(void*);
static unsigned char* arena;
static unsigned char* owner;
static unsigned mode;
static FILE* output;
static void record(unsigned kind,void* self,unsigned argument){unsigned h[]={kind,(unsigned)self,argument};fwrite(h,4,3,output);fwrite(arena,1,16384,output);}
extern "C" bool __fastcall Finished(void* self){
    record(1,self,0);if(mode==3){unsigned char* node=*(unsigned char**)((unsigned char*)self+4);*(void**)(node+32)=0;}
    return mode==0;
}
extern "C" void __fastcall BankDiscard(void* self,void*,void* pointer){
    record(2,self,(unsigned)pointer);
    if(mode==1)*(unsigned*)((unsigned char*)pointer-8)=99;
}
extern "C" void __fastcall DeleteData(void* self){
    record(3,self,0);unsigned char* node=*(unsigned char**)self;memset(self,0x93,32);
    if(mode==2)*(void**)(node+32)=arena+0x3e00;
}
void operator delete(void* p){
    if(arena&&(unsigned char*)p>=arena&&(unsigned char*)p<arena+16384){record(4,p,0);memset(p,0xdd,12);}
    else free(p);
}
extern "C" void __cdecl BlockFree(void* p){record(5,p,0);memset(p,0xde,36);if(mode==2)*(unsigned*)(owner+12)+=3;}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned at=retailFixups[i][0],target=retailFixups[i][1];void* destination=0;
        switch(target){
        case 0x9d54e0:destination=(void*)Finished;break;
        case 0x9d68a0:destination=code+256;break;
        case 0x9d67e0:destination=(void*)BankDiscard;break;
        case 0x42a34e:destination=code+512;break;
        case 0xbfe9bc:destination=(void*)(void(*)(void*))operator delete;break;
        case 0xbfea14:destination=(void*)BlockFree;break;
        case 0x41c7e0:destination=code+1152;break;
        case 0x41c800:destination=code+1184;break;
        case 0x42951b:destination=code+1216;break;
        case 0x42955b:destination=code+1280;break;
        default:return 9;
        }
        *(unsigned*)(code+at)=(unsigned)destination-(unsigned)(code+at+4);
    }
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,16384,in)!=16384)return 5;mode=p[0];owner=arena+p[1];
        for(unsigned i=0;i<7;++i)*(unsigned*)(arena+0x3000+32*i+4)=(unsigned)DeleteData;
        *(unsigned*)(arena+0x3e04)=(unsigned)DeleteData;
        entry(owner);record(8,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_UNUSED_BLOCKS_PASS cases=%u\n",cases);return 0;
}
