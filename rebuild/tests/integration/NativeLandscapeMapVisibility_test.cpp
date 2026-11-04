#include <stdio.h>
#include <string.h>
#include "landscape_visibility_aliases.h"
#include "landscape_visibility_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,bool);
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void set(unsigned offset,unsigned value){*(unsigned*)(arena+offset)=value;}
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall Discard(void* self,void*,void* slot){
    record(1,self,(unsigned)slot);memset(slot,0,8);
    if(mode==1)arena[0x5d]=1;
    if(mode==2)arena[0x5d]=0;
}
static void patchMutation(){
    if(mode==3){set(0x30,0x80000001);arena[0x5e]=1;}
    if(mode==4){set(4,(unsigned)(arena+0x900));set(0x20,(unsigned)(arena+0xb00));}
}
extern "C" void __cdecl ArrayFree(void* self){record(2,self,0);memset(self,0x92,4);patchMutation();}
void __cdecl operator delete[](void* self){ArrayFree(self);}
extern "C" void* __fastcall PatchDelete(void* self,void*,unsigned flags){record(3,self,flags);memset(self,0x93,0x48);patchMutation();return self;}
extern "C" void __fastcall Release(void* self,void*,long key){record(4,self,(unsigned)key);if(mode==5)set(4,(unsigned)(arena+0x900));arena[0x5e]=1;}
extern "C" void* __fastcall TreeDelete(void* self,void*,unsigned flags){record(5,self,flags);memset(self,0x95,0xe8);if(mode==6)set(0x20,0);if(mode==7)set(0x20,(unsigned)(arena+0xb00));return self;}
extern "C" void __fastcall Finish(void* self){record(6,self,0);memset(self,0x96,32);}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;
    memcpy(code,retailCode,sizeof(retailCode));memcpy(code+256,discardCode,sizeof(discardCode));
    *(unsigned*)(code+22)=(unsigned)(code+256)-(unsigned)(code+26);
    *(unsigned*)(code+60)=(unsigned)ArrayFree-(unsigned)(code+64);
    *(unsigned*)(code+89)=(unsigned)Release-(unsigned)(code+93);
    *(unsigned*)(code+125)=(unsigned)Finish-(unsigned)(code+129);
    *(unsigned*)(code+267)=(unsigned)Discard-(unsigned)(code+271);
    typedef void (__fastcall *Call)(void*,void*,bool);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];
        set(0xd00,(unsigned)PatchDelete);set(0xd10,(unsigned)TreeDelete);
        entry(arena,0,p[1]!=0);record(7,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_LANDSCAPE_VISIBILITY_PASS cases=%u\n",cases);return 0;
}
