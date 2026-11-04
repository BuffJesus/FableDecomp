#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "fable_engine_internal.h"
#include "detail_loading_base_aliases.h"
#include "detail_loading_base_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Load(void*,void*,void*);
extern "C" unsigned TypeTable[],BaseTable[],RootTable[];
namespace NEngineInternal {CEngineLocalDetailGenerator* GPLocalDetailGenerator;CEngineResourceManager* GPResourceManager;}
static unsigned char* arena;static FILE* output;static unsigned mode,events,loaded,names,allocations,vectorAllocations;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>60)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){record(1,self,n);if(n!=1&&n!=4&&n!=52)exit(21);memcpy(out,(void*)at(0x20c),n);at(0x20c)+=n;at(0x204)+=n;at(0x214)=0;if(n==52){((unsigned*)out)[-4]=0x12345678;if(mode==2)NEngineInternal::GPLocalDetailGenerator=(CEngineLocalDetailGenerator*)(arena+0x440);}if(mode==1){at(0)=(unsigned)(arena+0x600);at(4)=at(0)+8;at(8)=at(0)+16;}}
extern "C" void* __fastcall ReadName(void* self,void*,void* result){unsigned i=names++;*(unsigned*)result=(unsigned)(arena+0xc00+(i%3)*32);record(4,self,i);return result;}
extern "C" void __fastcall DestroyString(void* self){record(8,0,*(unsigned*)self);}
extern "C" unsigned __fastcall Find(void* self,void*,const void* name){record(9,self,*(unsigned*)name);return ((*(unsigned*)name-(unsigned)(arena+0xc00))/32)+1;}
void* __cdecl operator new(unsigned n){if(n!=84||allocations>=3)exit(26);record(12,0,n);return arena+0xa00+96*allocations++;}
extern "C" void* __cdecl Allocate(unsigned n){if(n>32||vectorAllocations>=8)exit(28);record(14,0,n);return arena+0x900+32*vectorAllocations++;}
extern "C" void __cdecl Free(void* p){record(15,p,0);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x993ca0:v=(unsigned)Overflow;break;case 0xbfeae6:v=(unsigned)memmove;break;case 0x99a2f0:v=(unsigned)(code+2560);break;case 0x99a300:v=(unsigned)(code+2576);break;case 0x12a7d94:v=(unsigned)TypeTable;break;case 0xbe27b0:v=(unsigned)(code+512);break;case 0xb59c70:v=(unsigned)(code+1408);break;case 0xbe2680:v=(unsigned)(code+1792);break;case 0xbe26c0:v=(unsigned)(code+1664);break;case 0xbfea1a:v=(unsigned)&operator new;break;case 0xb59f60:v=(unsigned)(code+1920);break;case 0xbdfc00:v=(unsigned)(code+2176);break;case 0x1436e88:v=(unsigned)&NEngineInternal::GPLocalDetailGenerator;break;case 0x99eae0:v=(unsigned)DestroyString;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0xa24280:v=(unsigned)(code+1280);break;case 0xa24290:v=(unsigned)(code+1296);break;case 0x1436e98:v=(unsigned)&NEngineInternal::GPResourceManager;break;case 0xbfea0e:v=(unsigned)Allocate;break;case 0xbfea14:v=(unsigned)Free;break;case 0x129a7c4:v=(unsigned)BaseTable;break;case 0x1231710:v=(unsigned)RootTable;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v:v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:Load;
 unsigned p,cases=0;while(fread(&p,4,1,in)==1){if(fread(arena,1,4096,in)!=4096)return 5;mode=p;events=loaded=names=allocations=vectorAllocations=0;NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0xd00);at(0x200)=(unsigned)(arena+0xe00);at(0xe18)=(unsigned)ReadName;NEngineInternal::GPLocalDetailGenerator=(CEngineLocalDetailGenerator*)(arena+0x400);fn(arena,0,arena+0x200);record(7,NEngineInternal::GPLocalDetailGenerator,loaded);++cases;}
 fclose(in);fclose(output);printf("NATIVE_DETAIL_LOADING_BASE_PASS cases=%u\n",cases);return 0;
}
