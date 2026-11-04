#include "engine/CEngineWorldMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "fable_engine_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_creation_aliases.h"
#include "map_creation_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Add(void*,void*,long,long);
extern "C" void __fastcall DefaultText(void*);
extern "C" void __fastcall CopyText(void*,void*,const void*);
extern "C" void __fastcall AssignText(void*,void*,const void*);
extern "C" void __fastcall DestroyText(void*);
extern "C" void __fastcall LandscapeCreate(void*,void*,void*,long);
extern "C" void __fastcall DetailCreate(void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,id,children,newCount,treeCount,nodeCount,events;
extern "C" void* EngineDef;extern "C" void* Resources;extern "C" unsigned LandscapeTable[],QuadTable[];
extern "C" void __fastcall FreeVirtual(void*,void*,void*){exit(24);}
static void record(unsigned kind,void* owner,unsigned arg){if(++events>150)exit(20);unsigned h[]={kind,(unsigned)owner,arg,(unsigned)NEngineInternal::GPLandscapeRenderer,(unsigned)NEngineInternal::GPLocalDetailGenerator};if(fwrite(h,4,5,output)!=5||fwrite(arena,1,8192,output)!=8192)exit(9);}
static CEngineMap*& slot(){return (*(CEngineMap***)(arena+0x20))[id];}
static void swapVector(){unsigned size=*(unsigned*)(arena+0x24)-*(unsigned*)(arena+0x20);memcpy(arena+0x740,*(void**)(arena+0x20),16);*(void**)(arena+0x20)=arena+0x740;*(void**)(arena+0x24)=arena+0x740+size;*(void**)(arena+0x28)=arena+0x750;}
extern "C" void* __fastcall Info(void* self,void*,void* result,long index){if(index!=(long)id)exit(10);record(1,self,index);memcpy(result,arena+0x1800,32);if(mode==1)swapVector();return result;}
extern "C" void __fastcall Resize(void* self,void*,unsigned n,void* const* value){record(2,self,n);if(*value||n>4)exit(11);unsigned* p=(unsigned*)self;for(unsigned* v=(unsigned*)p[1];v<(unsigned*)(p[0]+n*4);++v)*v=0;p[1]=p[0]+n*4;}
namespace _STL {template<> inline void vector<CEngineMap*>::resize(size_t n,CEngineMap* const& value){Resize(this,0,n,(void* const*)&value);}}
void CEngineWorldMap::DeleteMap(long index){record(3,this,index);if(index!=(long)id)exit(12);slot()=0;if(mode==2)swapVector();}
CCharString::CCharString(){record(4,0,0);*(unsigned*)this=0;}
CCharString::CCharString(const CCharString& other){record(5,0,*(unsigned*)&other);*(unsigned*)this=*(unsigned*)&other;}
CCharString& CCharString::operator=(const CCharString& other){record(6,this,*(unsigned*)&other);*(unsigned*)this=*(unsigned*)&other;return *this;}
CCharString::~CCharString(){record(7,0,*(unsigned*)this);}
void* __cdecl operator new(unsigned size){record(8,0,size);if(size==0x68){if(newCount++)exit(13);return arena+0x1000;}if(size==0x60)return children&1?arena+0x1200:0;if(size==0x70)return children&2?arena+0x1300:0;exit(21);return 0;}
void __cdecl operator delete(void* p){if(output)record(9,p,0);}
extern "C" void* __cdecl MapAllocate(unsigned size){record(10,0,size);if(size==8){if(mode==3)*(unsigned short*)(arena+0x105c)=17;return arena+0x1d00;}if(size==36){if(treeCount>1)exit(14);return arena+0x1400+0x40*treeCount++;}if(size==12){if(nodeCount>1)exit(15);if(mode==6)*(void**)(arena+0x2c)=arena+0x920;return arena+0x1500+0x20*nodeCount++;}exit(16);return 0;}
extern "C" void __cdecl CreationFree(void* p){record(17,p,0);}
extern "C" void __fastcall LandscapeVisible(void* self,void*,bool visible){if(!visible)exit(17);record(13,self,visible);if(mode==5)slot()->LocalDetailCacheMap=0;}
extern "C" void __fastcall DetailVisible(void* self,void*,bool visible){if(!visible)exit(18);record(14,self,visible);}
extern "C" void __fastcall DeleteOne(void*,void*,long);
int main(int argc,char** argv){
 void (_STL::vector<CEngineMap*>::*volatile resizeBoundary)(size_t,CEngineMap* const&)=&_STL::vector<CEngineMap*>::resize;
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
  case 0xb429e0:v=(unsigned)(code+512);break;case 0xbfeb1c:v=(unsigned)&operator delete;break;case 0xbfea1a:v=(unsigned)&operator new;break;case 0xbfea0e:v=(unsigned)MapAllocate;break;case 0xb44170:v=(unsigned)Resize;break;case 0xb42b10:v=(unsigned)DeleteOne;break;
  case 0x99e4b0:v=(unsigned)DefaultText;break;case 0x99ec30:v=(unsigned)CopyText;break;case 0x99efb0:v=(unsigned)AssignText;break;case 0x99eae0:v=(unsigned)DestroyText;break;
  case 0xb67140:v=(unsigned)(code+704);break;case 0xb59800:v=(unsigned)(code+768);break;case 0xbdd2f0:v=(unsigned)(code+832);break;case 0xbdefc0:v=(unsigned)(code+1024);break;case 0xbddf30:v=(unsigned)(code+1120);break;case 0xbf90a0:v=(unsigned)(code+1280);break;case 0xbf94d0:v=(unsigned)(code+1472);break;case 0xbf8f30:v=(unsigned)(code+1728);break;case 0xbf8f70:v=(unsigned)(code+1792);break;case 0x12a7d6c:v=(unsigned)LandscapeTable;break;case 0x12a7df0:v=(unsigned)QuadTable;break;case 0x1436e20:v=(unsigned)&EngineDef;break;case 0x1436e98:v=(unsigned)&Resources;break;case 0xbfe9bc:v=(unsigned)&operator delete;break;case 0xbfea14:v=(unsigned)CreationFree;break;case 0xbdc3c0:v=(unsigned)LandscapeVisible;break;case 0xbde160:v=(unsigned)DetailVisible;break;
  case 0x1436ea8:v=(unsigned)&NEngineInternal::GPLandscapeRenderer;break;case 0x1436e88:v=(unsigned)&NEngineInternal::GPLocalDetailGenerator;break;default:return 8;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Call)(void*,void*,long,long);Call run=argv[3][0]=='r'?(Call)code:Add;
 unsigned p[4],cases=0;while(fread(p,4,4,in)==4){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];id=p[1];children=p[3];newCount=treeCount=nodeCount=events=0;NEngineInternal::GPLandscapeRenderer=(CEngineLandscapeRenderer*)(arena+0x1a00);NEngineInternal::GPLocalDetailGenerator=(CEngineLocalDetailGenerator*)(arena+0x1b00);*(void**)(arena+0x604)=(void*)Info;EngineDef=arena+0x1c00;Resources=arena+0x1000;*(void**)(arena+0x1954)=arena+0x1e00;*(unsigned*)(arena+0x1c78)=0x88776655;
  run(arena,0,id,p[2]);record(15,0,0);run(arena,0,id,p[2]);record(16,0,0);++cases;}
 fclose(in);fclose(output);output=0;printf("NATIVE_MAP_CREATION_PASS cases=%u\n",cases);return 0;
}
