#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quad_lifetime_aliases.h"
#include "quad_lifetime_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Destroy(void*);
extern "C" void* __fastcall DeleteQuad(void*,void*,unsigned);
extern "C" void* __fastcall DeleteBase(void*,void*,unsigned);
extern "C" bool __fastcall CanQuad(void*,void*,long);
extern "C" bool __fastcall CanBase(void*,void*,long);
extern "C" unsigned long __fastcall VerifyBase(void*);
extern "C" void* Renderer;
extern "C" unsigned BaseTable[],QuadTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg,(unsigned)Renderer};if(fwrite(h,4,4,output)!=4||fwrite(arena,1,8192,output)!=8192)exit(9);}
extern "C" void* __fastcall ChildDelete(void* self,void*,unsigned flags){record(1,self,flags);if(flags!=1)exit(10);if(mode==1)*(void**)(arena+20)=arena+0x500;if(mode==2)*(void**)(arena+16)=0;return self;}
extern "C" void* __fastcall CollectionDelete(void* self,void*,unsigned flags){record(2,self,flags);if(flags!=1)exit(11);if(mode==3)*(void**)(arena+8)=arena+0x200;if(mode==4)*(void**)(arena+4)=0;return self;}
extern "C" void __fastcall FreeVirtual(void* self,void*,void* p){record(3,self,(unsigned)p);}
void __cdecl operator delete(void* p){if(output)record(4,p,0);}
int main(int argc,char** argv){
 if(argc!=4||Renderer)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],target=retailFixups[i][1],v=0;
  switch(target){case 0x129b860:v=(unsigned)BaseTable;break;case 0x12a7df0:v=(unsigned)QuadTable;break;case 0xbe0400:v=(unsigned)(code+384);break;case 0xbdda50:v=(unsigned)(code+320);break;case 0xbddfb0:v=(unsigned)code;break;case 0x1436ea8:v=(unsigned)&Renderer;break;case 0xbd9b90:v=(unsigned)FreeVirtual;break;case 0xbfe9bc:v=(unsigned)&operator delete;break;case 0xbe08e0:v=(unsigned)(code+448);break;default:return 13;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Dtor)(void*);typedef void* (__fastcall *Del)(void*,void*,unsigned);typedef bool (__fastcall *Can)(void*,void*,long);typedef unsigned long (__fastcall *Verify)(void*);
 Dtor destroy=argv[3][0]=='r'?(Dtor)code:Destroy;Del quad=argv[3][0]=='r'?(Del)(code+128):DeleteQuad;Del base=argv[3][0]=='r'?(Del)(code+192):DeleteBase;Can cq=argv[3][0]=='r'?(Can)(code+256):CanQuad;Can cb=argv[3][0]=='r'?(Can)(code+288):CanBase;Verify verify=argv[3][0]=='r'?(Verify)(code+544):VerifyBase;
 unsigned p[3],cases=0;
 while(fread(p,4,3,in)==3){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];Renderer=arena+0x1000;*(void**)(arena+0x1800)=(void*)ChildDelete;*(void**)(arena+0x1820)=(void*)CollectionDelete;
  if(cq(arena,0,-1)!=!arena[0x800]||!cb(arena,0,0x7fffffff)||verify(arena)!=0x7b)return 6;
  if(p[1]==0)destroy(arena);else if(p[1]==1){if(quad(arena,0,p[2])!=arena)return 7;}else if(base(arena,0,p[2])!=arena)return 8;
  record(5,0,p[2]);++cases;
 }
 fclose(in);fclose(output);output=0;printf("NATIVE_QUAD_LIFETIME_PASS cases=%u\n",cases);return 0;
}

