#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_dispatch_aliases.h"
#include "primitive_dispatch_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u step %u\n",__LINE__,cases,step);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* PrimitiveGlobal;
extern "C" void __fastcall Add(void*,void*,void*);
extern "C" void __fastcall Persistent(void*,void*,void*,void*,unsigned);
extern "C" unsigned __fastcall AddChild(void*,void*,void*,void*,unsigned);
extern "C" void __fastcall SetChildHandle(void*,void*,void*,unsigned,void*,void*,unsigned);
extern "C" void __fastcall GetChild(void*,void*,void*,unsigned,void*);
extern "C" void __fastcall SetChild(void*,void*,void*,unsigned,void*,unsigned);
extern "C" void __fastcall ClearChild(void*,void*,void*,unsigned);
extern "C" void __fastcall ClearAll(void*,void*,void*);
static unsigned char *arena,expected[1024];static unsigned p[6],cases,step,calls;static void *handle,*primitive,*child,*expectedGlobal;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,1024)&&PrimitiveGlobal==expectedGlobal);fwrite(arena,1,1024,output);fwrite(&PrimitiveGlobal,4,1,output);}
static void Call(void*self,unsigned which){REQUIRE(step==which&&calls++==which&&self==expectedGlobal);Record();if(p[4]==1){W(arena,0x104)^=p[2];W(expected,0x104)^=p[2];}if(p[4]==2){unsigned off=(unsigned char*)child-arena;W(arena,off)=W(expected,off)=p[1];}PrimitiveGlobal=expectedGlobal=p[4]==3?0:arena+0x200+32*((p[0]+which+1)%2);Record();}
extern "C" void __fastcall OnAdd(void*self,void*,void*v){REQUIRE(v==primitive);Call(self,0);}
extern "C" void __fastcall OnPersistent(void*self,void*,void*h,void*v,unsigned flags,unsigned hidden){REQUIRE(h==handle&&v==primitive&&flags==p[2]&&hidden==0);Call(self,1);}
extern "C" unsigned __fastcall OnAddChild(void*self,void*,void*h,void*v,unsigned flags){REQUIRE(h==handle&&v==primitive&&flags==p[2]);Call(self,2);return p[5];}
extern "C" void __fastcall OnSetChildHandle(void*self,void*,void*h,unsigned index,void*c,void*v,unsigned flags){REQUIRE(h==handle&&index==p[1]&&c==child&&v==primitive&&flags==p[2]);Call(self,3);}
extern "C" void __fastcall OnGetChild(void*self,void*,void*h,unsigned index,void*c){REQUIRE(h==handle&&index==p[1]&&c==child);Call(self,4);}
extern "C" void __fastcall OnSetChild(void*self,void*,void*h,unsigned index,void*v,unsigned flags){REQUIRE(h==handle&&index==p[1]&&v==primitive&&flags==p[2]);Call(self,5);}
extern "C" void __fastcall OnClearChild(void*self,void*,void*h,unsigned index){REQUIRE(h==handle&&index==p[1]);Call(self,6);}
extern "C" void __fastcall OnClearAll(void*self,void*,void*h){REQUIRE(h==handle);Call(self,7);}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<16;++i){unsigned v;switch(retailFixups[i][1]){case 0x1436e80:v=(unsigned)&PrimitiveGlobal;break;case 0xb32e90:v=(unsigned)&OnAdd;break;case 0xb324a0:v=(unsigned)&OnPersistent;break;case 0xb31ef0:v=(unsigned)&OnAddChild;break;case 0xb31f20:v=(unsigned)&OnSetChildHandle;break;case 0xb31f50:v=(unsigned)&OnGetChild;break;case 0xb31f70:v=(unsigned)&OnSetChild;break;case 0xb31fa0:v=(unsigned)&OnClearChild;break;case 0xb31fc0:v=(unsigned)&OnClearAll;break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 bool native=!strcmp(argv[3],"retail");
 while(fread(p,4,6,in)==6){if(fread(arena,1,1024,in)!=1024)return 6;memcpy(expected,arena,1024);PrimitiveGlobal=expectedGlobal=arena+0x200+32*p[0];handle=arena+0x100;primitive=p[3]==1?handle:arena+0x120;child=p[3]==2?handle:primitive;calls=0;
  step=0;(native?(void(__fastcall*)(void*,void*,void*))code:&Add)(arena,0,primitive);
  step=1;(native?(void(__fastcall*)(void*,void*,void*,void*,unsigned))(code+32):&Persistent)(arena,0,handle,primitive,p[2]);
  step=2;unsigned result=(native?(unsigned(__fastcall*)(void*,void*,void*,void*,unsigned))(code+64):&AddChild)(arena,0,handle,primitive,p[2]);REQUIRE(result==p[5]);
  step=3;(native?(void(__fastcall*)(void*,void*,void*,unsigned,void*,void*,unsigned))(code+96):&SetChildHandle)(arena,0,handle,p[1],child,primitive,p[2]);
  step=4;(native?(void(__fastcall*)(void*,void*,void*,unsigned,void*))(code+128):&GetChild)(arena,0,handle,p[1],child);
  step=5;(native?(void(__fastcall*)(void*,void*,void*,unsigned,void*,unsigned))(code+160):&SetChild)(arena,0,handle,p[1],primitive,p[2]);
  step=6;(native?(void(__fastcall*)(void*,void*,void*,unsigned))(code+192):&ClearChild)(arena,0,handle,p[1]);
  step=7;(native?(void(__fastcall*)(void*,void*,void*))(code+224):&ClearAll)(arena,0,handle);REQUIRE(calls==8);Record();++cases;
 }fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_DISPATCH_PASS cases=%u\n",cases);return 0;
}
