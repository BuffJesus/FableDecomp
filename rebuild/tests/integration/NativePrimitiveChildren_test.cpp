#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_children_aliases.h"
#include "primitive_children_retail.h"
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
extern "C" unsigned __fastcall ChildAddChild(void*,void*,void*,void*,unsigned);
extern "C" void __fastcall ChildSetChildHandle(void*,void*,void*,unsigned,void*,void*,unsigned);
extern "C" void __fastcall ChildGetChild(void*,void*,void*,unsigned,void*);
extern "C" void __fastcall ChildSetChild(void*,void*,void*,unsigned,void*,unsigned);
extern "C" void __fastcall ChildClearChild(void*,void*,void*,unsigned);
extern "C" void __fastcall ChildClearAll(void*,void*,void*);
static unsigned char *arena,expected[1024];static unsigned p[8],cases,step,calls;static void *handle,*primitive,*child,*expectedGlobal,*expectedReceiver;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,1024)&&PrimitiveGlobal==expectedGlobal);fwrite(arena,1,1024,output);fwrite(&PrimitiveGlobal,4,1,output);}
static void Call(void*self,unsigned which){
 REQUIRE(step==which&&self==expectedReceiver&&self!=0);++calls;Record();
 if(p[4]==1)W(arena,0x104)=W(expected,0x104)=0;
 if(p[4]==2)W(arena,0x104)=W(expected,0x104)=(unsigned)(arena+(self==arena+0x280?0x300:0x280));
 if(p[4]==3){unsigned off=(unsigned char*)child-arena;W(arena,off)=W(expected,off)=p[1];}
 PrimitiveGlobal=expectedGlobal=arena+0x200+32*((p[0]+which+1)%2);Record();
}
extern "C" void __fastcall OnAdd(void*,void*,void*){exit(81);}
extern "C" void __fastcall OnPersistent(void*,void*,void*,void*,unsigned,unsigned){exit(82);}
static unsigned __fastcall VAddChild(void*self,void*,void*v,unsigned flags){REQUIRE(v==primitive&&flags==p[2]);Call(self,0);return p[5];}
static void __fastcall VSetChildHandle(void*self,void*,unsigned index,void*c,void*v,unsigned flags){REQUIRE(index==p[1]&&c==child&&v==primitive&&flags==p[2]);Call(self,1);}
static void __fastcall VGetChild(void*self,void*,unsigned index,void*c){REQUIRE(index==p[1]&&c==child);Call(self,2);}
static void __fastcall VSetChild(void*self,void*,unsigned index,void*v,unsigned flags){REQUIRE(index==p[1]&&v==primitive&&flags==p[2]);Call(self,3);}
static void __fastcall VClearChild(void*self,void*,unsigned index){REQUIRE(index==p[1]);Call(self,4);}
static void __fastcall VClearAll(void*self,void*){Call(self,5);}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<16;++i){unsigned v;switch(retailFixups[i][1]){case 0x1436e80:v=(unsigned)&PrimitiveGlobal;break;case 0xb32e90:v=(unsigned)&OnAdd;break;case 0xb324a0:v=(unsigned)&OnPersistent;break;case 0xb31ef0:v=(unsigned)(code+256);break;case 0xb31f20:v=(unsigned)(code+320);break;case 0xb31f50:v=(unsigned)(code+384);break;case 0xb31f70:v=(unsigned)(code+448);break;case 0xb31fa0:v=(unsigned)(code+512);break;case 0xb31fc0:v=(unsigned)(code+576);break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 static void* table[12];table[6]=(void*)&VAddChild;table[7]=(void*)&VSetChildHandle;table[8]=(void*)&VSetChild;table[9]=(void*)&VGetChild;table[10]=(void*)&VClearChild;table[11]=(void*)&VClearAll;
 bool native=!strcmp(argv[3],"retail");
 while(fread(p,4,8,in)==8){if(fread(arena,1,1024,in)!=1024)return 6;W(arena,0x104)=p[6]?(unsigned)(arena+(p[6]==1?0x280:0x300)):0;W(arena,0x280)=W(arena,0x300)=(unsigned)table;memcpy(expected,arena,1024);PrimitiveGlobal=expectedGlobal=arena+0x200+32*p[0];handle=arena+0x100;primitive=p[3]==1?handle:arena+0x120;child=p[3]==2?handle:primitive;calls=0;
  unsigned wanted=0;
  for(step=0;step<6;++step){expectedReceiver=(void*)W(expected,0x104);if(expectedReceiver)++wanted;void* function=0;
   unsigned off=p[7]?64+32*step:256+64*step;
   switch(step){
    case 0:{typedef unsigned(__fastcall*Fn)(void*,void*,void*,void*,unsigned);Fn fn=native?(Fn)(code+off):p[7]?&AddChild:&ChildAddChild;unsigned result=fn(arena,0,handle,primitive,p[2]);REQUIRE(result==(expectedReceiver?p[5]:0));break;}
    case 1:{typedef void(__fastcall*Fn)(void*,void*,void*,unsigned,void*,void*,unsigned);Fn fn=native?(Fn)(code+off):p[7]?&SetChildHandle:&ChildSetChildHandle;fn(arena,0,handle,p[1],child,primitive,p[2]);break;}
    case 2:{typedef void(__fastcall*Fn)(void*,void*,void*,unsigned,void*);Fn fn=native?(Fn)(code+off):p[7]?&GetChild:&ChildGetChild;fn(arena,0,handle,p[1],child);break;}
    case 3:{typedef void(__fastcall*Fn)(void*,void*,void*,unsigned,void*,unsigned);Fn fn=native?(Fn)(code+off):p[7]?&SetChild:&ChildSetChild;fn(arena,0,handle,p[1],primitive,p[2]);break;}
    case 4:{typedef void(__fastcall*Fn)(void*,void*,void*,unsigned);Fn fn=native?(Fn)(code+off):p[7]?&ClearChild:&ChildClearChild;fn(arena,0,handle,p[1]);break;}
    case 5:{typedef void(__fastcall*Fn)(void*,void*,void*);Fn fn=native?(Fn)(code+off):p[7]?&ClearAll:&ChildClearAll;fn(arena,0,handle);break;}
   }REQUIRE(calls==wanted);Record();
  }++cases;
 }fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_CHILDREN_PASS cases=%u\n",cases);return 0;
}
