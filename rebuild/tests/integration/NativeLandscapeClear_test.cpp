#include <stdio.h>
#include <string.h>
#include "landscape_clear_aliases.h"
#include "landscape_clear_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*);
extern "C" void __fastcall Close(void*);
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void put(unsigned off,unsigned value){*(unsigned*)(arena+off)=value;}
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
static void patchMutation(){if(mode==1)put(4,(unsigned)(arena+0x900));}
extern "C" void __cdecl ArrayFree(void* self){record(1,self,0);memset(self,0x91,4);patchMutation();}
void __cdecl operator delete[](void* self){ArrayFree(self);}
extern "C" void* __fastcall PatchDelete(void* self,void*,unsigned flags){record(2,self,flags);memset(self,0x92,0x48);patchMutation();return self;}
extern "C" void* __fastcall TreeDelete(void* self,void*,unsigned flags){record(3,self,flags);memset(self,0x93,0xe8);if(mode==2)put(0x1c,(unsigned)(arena+0xe40));return self;}
extern "C" void __fastcall DeleteData(void* self){record(4,self,0);if(mode==3)put(0x1c,(unsigned)(arena+0xe40));}
extern "C" void __cdecl FreeInfo(void* self){record(5,self,0);if(self)memset(self,0x95,12);if(mode==4){put(0x18,0xdeadbeef);put(0x1c,0xdeadbeef);arena[0x5d]=1;}}
void __cdecl operator delete(void* self){FreeInfo(self);}
extern "C" void __fastcall Discard(void* self,void*,void* slot){record(6,self,(unsigned)slot);memset(slot,0,8);if(mode==5){arena[0x5e]=1;put(0x30,0x80000000);put(0x20,(unsigned)(arena+0xc40));}}
extern "C" void __fastcall Release(void* self,void*,long key){record(7,self,(unsigned)key);if(mode==6){memset(arena+0x2c,0x97,12);arena[0x5e]=1;}}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned* table=(unsigned*)VirtualAlloc((void*)0x25000000,4096,0x3000,4);if(table!=(void*)0x25000000)return 7;table[0]=(unsigned)PatchDelete;table[1]=(unsigned)TreeDelete;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+33)=(unsigned)ArrayFree-(unsigned)(code+37);*(unsigned*)(code+86)=(unsigned)FreeInfo-(unsigned)(code+90);
    *(unsigned*)(code+113)=(unsigned)(code+192)-(unsigned)(code+117);*(unsigned*)(code+130)=(unsigned)Release-(unsigned)(code+134);
    *(unsigned*)(code+203)=(unsigned)Discard-(unsigned)(code+207);*(unsigned*)(code+228)=(unsigned)code-(unsigned)(code+232);
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;Call close=argv[3][0]=='r'?(Call)(code+224):Close;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];put(0xe04,(unsigned)DeleteData);put(0xe44,(unsigned)DeleteData);
        if(p[1])close(arena);else entry(arena);record(8,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_LANDSCAPE_CLEAR_PASS cases=%u\n",cases);return 0;
}
