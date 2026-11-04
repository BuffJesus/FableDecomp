#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "landscape_lifetime_aliases.h"
#include "landscape_lifetime_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Construct(void*,void*,void*,long);
extern "C" void __fastcall Destroy(void*);
extern "C" void* __fastcall DeleteMap(void*,void*,unsigned);
extern "C" bool __fastcall CanRelocate(void*,void*,long);
extern "C" void __fastcall Relocate(void*,void*,long,void*,void*,long);
extern "C" void* EngineDef;extern "C" void* Resources;
extern "C" unsigned BaseTable[],LandscapeTable[];
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static void record(unsigned kind,void* self,unsigned arg){if(++events>100)exit(20);unsigned h[]={kind,(unsigned)self,arg,(unsigned)EngineDef,(unsigned)Resources};if(fwrite(h,4,5,output)!=5||fwrite(arena,1,8192,output)!=8192)exit(9);}
extern "C" void* __fastcall Palette(void* self,void*,void* bank,bool retail,unsigned standin){record(1,self,standin);if(self!=arena+0x40||bank!=arena+0x1e00||retail)exit(10);unsigned* p=(unsigned*)self;p[0]=(unsigned)bank;p[1]=(unsigned)(arena+0x800);p[2]=p[1]+16;p[3]=p[2];p[4]=standin;((unsigned char*)self)[20]=0;
 if(mode==5){*(unsigned short*)(arena+0x145c)=33;*(unsigned short*)(arena+0x145e)=65535;}
 // Reentrant palette construction can attach ownership before Reset(0).
 *(void**)(arena+0x1c)=arena+0x900;*(void**)(arena+0x3c)=arena+0x940;return self;
}
extern "C" void* __fastcall PatchDelete(void* self,void*,unsigned flags){if(flags!=3)exit(11);record(2,self,flags);if(mode==1)*(void**)(arena+4)=arena+0x740;return self;}
extern "C" void* __fastcall TreeDelete(void* self,void*,unsigned flags){if(flags!=1)exit(12);record(3,self,flags);if(mode==2){*(void**)(arena+0x44)=arena+0x860;*(void**)(arena+0x48)=arena+0x870;}return self;}
extern "C" void __fastcall DestroyString(void* self){record(4,self,*(unsigned*)self);if(mode==3)*(void**)(arena+0x3c)=arena+0x980;}
extern "C" void __fastcall DeleteOwned(void* self){record(5,self,0);if(mode==4)*(void**)self=arena+0x9c0;}
void __cdecl operator delete(void* p){if(output){record(6,p,0);if(mode==6&&p==arena+0x4fc)*(void**)(arena+4)=arena+0x740;}}
extern "C" void __cdecl LandFree(void* p){record(7,p,0);}
extern "C" void __fastcall FreeVirtual(void*,void*,void*){exit(13);}
int main(int argc,char** argv){
 if(argc!=4||EngineDef||Resources)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
  case 0x12a7d6c:v=(unsigned)LandscapeTable;break;case 0x129b860:v=(unsigned)BaseTable;break;case 0x1436e20:v=(unsigned)&EngineDef;break;case 0x1436e98:v=(unsigned)&Resources;break;case 0xbf90a0:v=(unsigned)Palette;break;case 0xbfe9bc:case 0xbfeb1c:v=(unsigned)&operator delete;break;case 0xbfea14:v=(unsigned)LandFree;break;case 0x99eae0:v=(unsigned)DestroyString;break;
  case 0xbdd210:v=(unsigned)(code+192);break;case 0xbe0400:v=(unsigned)(code+544);break;case 0xbe08e0:v=(unsigned)(code+576);break;default:return 8;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void* (__fastcall *Ctor)(void*,void*,void*,long);typedef void (__fastcall *Dtor)(void*);typedef void* (__fastcall *Del)(void*,void*,unsigned);typedef bool (__fastcall *Can)(void*,void*,long);typedef void (__fastcall *Move)(void*,void*,long,void*,void*,long);
 bool native=argv[3][0]=='r';Ctor ctor=native?(Ctor)code:Construct;Dtor dtor=native?(Dtor)(code+192):Destroy;Del del=native?(Del)(code+448):DeleteMap;Can can=native?(Can)(code+480):CanRelocate;Move move=native?(Move)(code+512):Relocate;
 unsigned p[4],cases=0;while(fread(p,4,4,in)==4){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];events=0;EngineDef=arena+0x1500;Resources=arena+0x1000;
  *(void**)arena=LandscapeTable;*(void**)(arena+0x1700)=(void*)PatchDelete;*(void**)(arena+0x1720)=(void*)TreeDelete;
  for(unsigned i=0;i<4;++i)*(void**)(arena+0x904+i*0x40)=(void*)DeleteOwned;
  if(p[1]==0){if(ctor(arena,0,arena+0x1400,p[3])!=arena)return 6;record(8,0,0);}
  if(can(arena,0,0x7fffffff)!=!arena[0x5c])return 7;move(arena,0,-1,arena+0x500,arena+0x600,-1);
  if(p[1]==2){if(del(arena,0,p[2])!=arena)return 14;}else dtor(arena);record(9,0,0);++cases;
 }
 fclose(in);fclose(output);output=0;printf("NATIVE_LANDSCAPE_LIFETIME_PASS cases=%u\n",cases);return 0;
}
