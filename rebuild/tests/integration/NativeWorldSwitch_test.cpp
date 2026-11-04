#include <stdio.h>
#include <stdlib.h>
#include "engine/CEngineWorldMap.h"
#include <string.h>
#include "world_switch_aliases.h"
#include "world_switch_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* MapGlobal;
extern "C" void __fastcall Entry(void*,void*,void*);
extern "C" void __fastcall DeleteAll(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,count,mask,loaded,calls;
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg,(unsigned)MapGlobal};fwrite(h,4,4,output);fwrite(arena,1,8192,output);}
extern "C" void* __fastcall WorldMap(void* self){record(1,self,0);return arena+0x600;}
extern "C" long __fastcall Count(void* self){record(2,self,0);return count;}
extern "C" void* __fastcall Info(void* self,void*,void* result,long id){
 record(3,self,id);memcpy(result,arena+0x1800,32);unsigned* p=(unsigned*)result;
 ((unsigned char*)result)[0]=(mask>>id)&1;p[1]=(loaded>>id)&1?(unsigned)(arena+0x1600+id*4):0;p[2]=0x77880000+id;
 ++calls;
 if(mode==1)*(void**)(arena+0x19c)=arena+0x620;
 if(mode==2)MapGlobal=arena+0x200;
 if(mode==3){*(unsigned*)(arena+0x24)+=0x740-(*(unsigned*)(arena+0x20)-(unsigned)arena);*(void**)(arena+0x20)=arena+0x740;}
 if(mode==4)*(void**)(arena+0x800+id*0x100)=arena+0x1500;
 return result;
}
extern "C" void __fastcall DestroyString(void* self){record(4,0,*(unsigned*)self);fwrite((unsigned char*)self-8,1,32,output);}
extern "C" void __fastcall DeleteOne(void* self,void*,long id){if(id<0||id>=5)exit(12);record(5,self,id);
 if(mode==5)*(void**)(arena+0x19c)=arena+0x620;
 if(mode==7 && id==0)*(unsigned*)(arena+0x24)=*(unsigned*)(arena+0x20)+8;
 if(mode==8 && id==0){*(void**)(arena+0x20)=arena+0x740;*(void**)(arena+0x24)=arena+0x754;}
}
extern "C" void __fastcall Fill(void* self,void*,void* position,unsigned n,void* const* value){record(12,self,n);if(n||*value)exit(11);}
void __cdecl operator delete(void* p){if(output){record(11,p,0);if(mode==6)*(void**)(arena+12)=arena+0x1910;}}
extern "C" void __fastcall Resize(void* self,void*,unsigned n,void* const* value){record(6,self,n);if(*value!=0)exit(9);unsigned* p=(unsigned*)self;p[1]=p[0]+n*4;}
namespace _STL {
template<> inline void vector<CEngineMap*>::resize(size_t n,CEngineMap* const& value){Resize(this,0,n,(void* const*)&value);}
template<> inline void vector<CEngineMap*>::_M_fill_insert(CEngineMap** position,size_t n,CEngineMap* const& value){Fill(this,0,position,n,(void* const*)&value);}
}
extern "C" void __fastcall AddMap(void* self,void*,long id,long flags){record(7,self,id);if(flags)exit(10);if(mode==6){*(unsigned*)(arena+0x24)+=0x740-(*(unsigned*)(arena+0x20)-(unsigned)arena);*(void**)(arena+0x20)=arena+0x740;}}
void* __cdecl operator new[](unsigned n){if(n>1536)exit(14);record(14,0,n);return arena+0x1a00;}
int main(int argc,char** argv){
 void (_STL::vector<CEngineMap*>::*volatile resizeBoundary)(size_t,CEngineMap* const&)=&_STL::vector<CEngineMap*>::resize;
 void (_STL::vector<CEngineMap*>::*volatile fillBoundary)(CEngineMap**,size_t,CEngineMap* const&)=&_STL::vector<CEngineMap*>::_M_fill_insert;
 if(argc!=4||MapGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],target=retailFixups[i][1],v=0;
  switch(target){case 0xb42c70:v=(unsigned)(code+304);break;case 0xb42b10:v=(unsigned)DeleteOne;break;case 0xbfeae6:v=(unsigned)memmove;break;case 0xb43f80:v=(unsigned)Fill;break;case 0xbfe9bc:v=(unsigned)&operator delete;break;case 0xb44170:v=(unsigned)Resize;break;case 0xb42d10:v=(unsigned)AddMap;break;case 0x99eae0:v=(unsigned)DestroyString;break;case 0x1436e8c:v=(unsigned)&MapGlobal;break;case 0xbdbfe0:v=(unsigned)(code+288);break;case 0xb40070:v=(unsigned)(code+512);break;case 0xbfeb22:v=(unsigned)&operator new[];break;case 0xb3edf0:v=(unsigned)(code+896);break;default:return 8;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Call)(void*,void*,void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;unsigned p[5],cases=0;
 while(fread(p,4,5,in)==5){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];count=p[1];mask=p[2];loaded=p[3];calls=0;MapGlobal=arena+p[4];
  *(void**)(arena+0x1708)=(void*)WorldMap;*(void**)(arena+0x1720)=(void*)Count;*(void**)(arena+0x1724)=(void*)Info;
  entry(arena,0,arena+0x500);record(9,0,calls);entry(arena,0,arena+0x500);record(10,0,calls);if(argv[3][0]=='r')((void (__fastcall *)(void*))(code+304))(arena);else DeleteAll(arena);record(13,0,0);++cases;
 }
 fclose(in);fclose(output);output=0;printf("NATIVE_WORLD_SWITCH_PASS cases=%u\n",cases);return 0;
}

