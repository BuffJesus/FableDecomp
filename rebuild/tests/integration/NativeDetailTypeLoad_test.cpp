#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_engine_internal.h"
#include "detail_type_aliases.h"
#include "detail_type_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Load(void*,void*,void*,bool);
namespace NEngineInternal {CEngineResourceManager* GPResourceManager;}
static unsigned char* arena;static FILE* output;static unsigned mode,events,names,pattern;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>40)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){record(1,self,n);if(n!=4&&n!=52)exit(21);memcpy(out,(void*)at(0x20c),n);at(0x20c)+=n;at(0x204)+=n;at(0x214)=64;if(mode==1)at(0x14)=0;}
extern "C" void* __fastcall ReadName(void* self,void*,void* result){unsigned i=names++;unsigned choice=(pattern>>i)&1;*(unsigned*)result=choice?(unsigned)(arena+0x800+i*32):0;record(2,self,i);if(mode==2)NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0x440);return result;}
extern "C" void __fastcall Destroy(void* self){record(3,0,*(unsigned*)self);if(mode==3)NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0x440);}
extern "C" unsigned __fastcall Find(void* self,void*,const void* name){unsigned i=(*(unsigned*)name-(unsigned)(arena+0x800))/32;record(4,self,i);return at(0x900+i*4);}
extern "C" const void* __fastcall Static(void* self,void*,unsigned index){record(5,self,index);if(mode==4)at(0x18)=0;if(mode==5){at(0x1c)=7;NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0x440);}return arena+0xa00;}
extern "C" const void* __fastcall Repeated(void* self,void*,unsigned index){record(6,self,index);if(mode==4)at(0x1c)=0;if(mode==5)NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0x440);return arena+0xa40;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x993ca0:v=(unsigned)Overflow;break;case 0x99eae0:v=(unsigned)Destroy;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0xa24280:v=(unsigned)Static;break;case 0xa24290:v=(unsigned)Repeated;break;case 0x1436e98:v=(unsigned)&NEngineInternal::GPResourceManager;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v:v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*,bool);Fn fn=argv[3][0]=='r'?(Fn)code:Load;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];pattern=p[1];events=names=0;NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0x400);at(0x200)=(unsigned)(arena+0x700);at(0x718)=(unsigned)ReadName;fn(arena,0,arena+0x200,p[2]!=0);record(7,NEngineInternal::GPResourceManager,names);++cases;}
 fclose(in);fclose(output);printf("NATIVE_DETAIL_TYPE_LOAD_PASS cases=%u\n",cases);return 0;
}
