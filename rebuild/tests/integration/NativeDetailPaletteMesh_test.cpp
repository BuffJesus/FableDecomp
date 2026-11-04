#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "fable_engine_internal.h"
#include "detail_palette_mesh_aliases.h"
#include "detail_palette_mesh_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Load(void*,void*,void*);
extern "C" unsigned TypeTable[];
namespace NEngineInternal {CEngineLocalDetailGenerator* GPLocalDetailGenerator;CEngineResourceManager* GPResourceManager;}
static unsigned char* arena;static FILE* output;static unsigned mode,events,loaded,names;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>60)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){record(1,self,n);if(n!=1&&n!=4&&n!=52)exit(21);memcpy(out,(void*)at(0x20c),n);at(0x20c)+=n;at(0x204)+=n;at(0x214)=32;if(mode==1){at(0)=(unsigned)(arena+0x600);at(4)=at(0)+8;at(8)=at(0)+16;}}
extern "C" void __fastcall BaseCtor(void* self){memset(self,0x76,84);if(mode==2)NEngineInternal::GPLocalDetailGenerator=(CEngineLocalDetailGenerator*)(arena+0x440);record(2,0,0);}
extern "C" void __fastcall BaseDtor(void* self){memcpy(arena+0x800,self,84);record(3,0,0);if(mode==3){at(0)=(unsigned)(arena+0x600);at(4)=at(0);at(8)=at(0)+16;}}
extern "C" void* __fastcall ReadName(void* self,void*,void* result){unsigned i=names++;*(unsigned*)result=(unsigned)(arena+0xc00+(i%3)*32);record(4,self,i);return result;}
extern "C" void __fastcall DestroyString(void* self){record(8,0,*(unsigned*)self);}
extern "C" unsigned __fastcall Find(void* self,void*,const void* name){record(9,self,*(unsigned*)name);return ((*(unsigned*)name-(unsigned)(arena+0xc00))/32)+1;}
extern "C" const void* __fastcall Register(void* self,void*,const void* type){++loaded;memcpy(arena+0x800,type,84);record(5,self,loaded);if(mode==5){at(0)=(unsigned)(arena+0x600);at(4)=at(0)+4;at(8)=at(0)+16;}return mode==6?0:arena+0xa00+loaded*84;}
extern "C" void __fastcall Grow(void* self,void*,void* pos,const void* value,const void*,unsigned count,bool atend){if(self!=arena||pos!=(void*)at(4)||count!=1||!atend)exit(24);unsigned bytes=at(4)-at(0);if(bytes>32)exit(25);record(6,self,*(unsigned*)value);memmove(arena+0x700,(void*)at(0),bytes);at(0)=(unsigned)(arena+0x700);at(4)=at(0)+bytes;at(8)=at(0)+64;*(unsigned*)at(4)=*(unsigned*)value;at(4)+=4;}
namespace _STL {template<> inline void vector<const CLocalDetailObjectCollectionType*>::_M_insert_overflow(const CLocalDetailObjectCollectionType** p,const CLocalDetailObjectCollectionType* const& x,const __true_type& tag,size_t n,bool end){Grow(this,0,p,&x,&tag,n,end);}}
int main(int argc,char** argv){
 void (_STL::vector<const CLocalDetailObjectCollectionType*>::*volatile keep)(const CLocalDetailObjectCollectionType**,const CLocalDetailObjectCollectionType* const&,const _STL::__true_type&,size_t,bool)=&_STL::vector<const CLocalDetailObjectCollectionType*>::_M_insert_overflow;
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x993ca0:v=(unsigned)Overflow;break;case 0xbfeae6:v=(unsigned)memmove;break;case 0x99a2f0:v=(unsigned)BaseCtor;break;case 0x99a300:v=(unsigned)BaseDtor;break;case 0x12a7d94:v=(unsigned)TypeTable;break;case 0xbe27b0:v=(unsigned)(code+512);break;case 0xb59c70:v=(unsigned)Register;break;case 0xbdfc00:v=(unsigned)Grow;break;case 0x1436e88:v=(unsigned)&NEngineInternal::GPLocalDetailGenerator;break;case 0x99eae0:v=(unsigned)DestroyString;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0xa24280:v=(unsigned)(code+1280);break;case 0xa24290:v=(unsigned)(code+1296);break;case 0x1436e98:v=(unsigned)&NEngineInternal::GPResourceManager;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v:v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:Load;
 unsigned p,cases=0;while(fread(&p,4,1,in)==1){if(fread(arena,1,4096,in)!=4096)return 5;mode=p;events=loaded=names=0;NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0xd00);at(0x200)=(unsigned)(arena+0xe00);at(0xe18)=(unsigned)ReadName;NEngineInternal::GPLocalDetailGenerator=(CEngineLocalDetailGenerator*)(arena+0x400);fn(arena,0,arena+0x200);record(7,NEngineInternal::GPLocalDetailGenerator,loaded);++cases;}
 fclose(in);fclose(output);printf("NATIVE_DETAIL_PALETTE_MESH_PASS cases=%u\n",cases);return 0;
}
