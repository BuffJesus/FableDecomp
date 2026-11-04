#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world_water_aliases.h"
#include "world_water_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall LoadWater(void*);
extern "C" unsigned WaterName=0;
extern "C" void* WaterRenderer=0;
static unsigned char* arena;static FILE* output;static unsigned mode,indexValue,dataSize,events,streamDtors;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static unsigned stable(const void* p){unsigned v=(unsigned)p;return v>=(unsigned)arena&&v<(unsigned)arena+4096?v:0;}
static void record(unsigned kind,unsigned a,unsigned b){if(++events>24)exit(10);unsigned h[]={kind,a,b};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" unsigned __fastcall Find(void* self,void*,const void* name){if(self!=arena+0x34||name!=&WaterName)exit(20);record(1,(unsigned)self,indexValue);if(mode==2)at(0x400+4*indexValue)=(unsigned)(arena+0x540);return indexValue;}
static void* makeVector(void* self,unsigned count,unsigned offset,unsigned kind){if(count>32)exit(21);unsigned* v=(unsigned*)self;v[0]=count?(unsigned)(arena+offset):0;v[1]=v[0]+count;v[2]=v[1];if(count)memset(arena+offset,0,count);record(kind,count,v[0]);return self;}
extern "C" void* __fastcall Chars(void* self,void*,unsigned count){void* out=makeVector(self,count,0x900,2);if(mode==1){at(0x504)=32;at(0x544)=32;}return out;}
extern "C" void* __fastcall Bytes(void* self,void*,unsigned count){return makeVector(self,count,0xa00,7);}
extern "C" void __fastcall ReadInfo(void* self,void*,unsigned offset,void* dest,unsigned size){if(offset||size>32)exit(22);record(3,(unsigned)self,size);memset(dest,0x41,size);}
extern "C" void* __fastcall Stream(void* self,void*,const void* data,unsigned size){unsigned* s=(unsigned*)self;s[0]=0x123675c;s[1]=0;s[2]=size;s[3]=(unsigned)data;s[4]=0;s[5]=size;s[6]=(unsigned)data;record(4,stable(data),size);return self;}
extern "C" void* __fastcall Async(void* self,void*,void* result,unsigned index){if(self!=arena+0x34||index!=indexValue)exit(23);record(5,(unsigned)self,index);unsigned* r=(unsigned*)result;r[0]=(unsigned)(arena+0x600);r[1]=at(0x804)?(unsigned)(arena+0x800):0;if(mode==3){at(0x604)=(unsigned)(arena+0xd00);at(0x608)=7;}return result;}
extern "C" unsigned __fastcall DataSize(void* self,void*,unsigned index){record(6,(unsigned)self,index);return dataSize;}
extern "C" void __fastcall ReadData(void* self,void*,unsigned offset,void* dest,unsigned size){if(self!=arena+0x600||offset||size!=dataSize)exit(24);record(8,(unsigned)self,size);memset(dest,0x62,size);if(mode==4)WaterRenderer=arena+0xc00;}
extern "C" void __fastcall Load(void* self,void*,void* data,void* info){if(self!=WaterRenderer)exit(25);unsigned* d=(unsigned*)data;unsigned* i=(unsigned*)info;record(9,(unsigned)self,d[2]);record(10,stable((void*)d[6]),stable((void*)i[6]));if(mode==5&&at(0x804))at(0x800)=2;d[1]=9;i[1]=13;}
extern "C" void __fastcall DestroyStream(void* self){unsigned* s=(unsigned*)self;record(11,s[1],s[2]);if(++streamDtors==1&&mode==6&&at(0x804))at(0x800)=1;}
extern "C" void __cdecl WaterFree(void* p){record(12,(unsigned)p,0);if(p!=arena+0x900&&p!=arena+0xa00)exit(26);memset(p,0xee,32);}
extern "C" void __fastcall Dispose(void* p){record(13,(unsigned)p,0);at(0x610)=0xdead;}
void __cdecl operator delete(void* p){record(14,(unsigned)p,0);if(p!=arena+0x800)exit(27);at(0x808)=0;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned n=0;n<17;++n){unsigned off=retailFixups[n][0],v=0;switch(retailFixups[n][1]){
 case 0x1436ec8:v=(unsigned)&WaterName;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0x411910:v=(unsigned)Chars;break;case 0x9cbe90:v=(unsigned)ReadInfo;break;case 0x994930:v=(unsigned)Stream;break;case 0x9d58d0:v=(unsigned)Async;break;case 0x9cc240:v=(unsigned)DataSize;break;case 0x514e20:v=(unsigned)Bytes;break;case 0x9d52b0:v=(unsigned)ReadData;break;case 0x1436e54:v=(unsigned)&WaterRenderer;break;case 0xb6daf0:v=(unsigned)Load;break;case 0x993b10:v=(unsigned)DestroyStream;break;case 0xbfea14:v=(unsigned)WaterFree;break;case 0xbfe9bc:v=(unsigned)&operator delete;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[n][2]?v:v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*);Fn fn=argv[3][0]=='r'?(Fn)code:LoadWater;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];indexValue=p[1];dataSize=p[2];events=streamDtors=0;WaterRenderer=arena+0xb00;if(at(0x804))at(0x804)=(unsigned)Dispose;fn(arena);record(15,(unsigned)WaterRenderer,0);++cases;}
 fclose(in);fclose(output);printf("NATIVE_WORLD_WATER_LOAD_PASS cases=%u\n",cases);return 0;
}
