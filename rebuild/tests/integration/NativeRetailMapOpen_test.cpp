#include "fable_counted_pointer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retail_map_aliases.h"
#include "retail_map_retail.h"
class CBankFileAsyncEntry;
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall OpenRetail(void*,void*,void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>40)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void* __fastcall Find(void* self,void*,const void* name){if(self!=arena+0x400||name!=arena+0x1c)exit(21);record(1,self,*(unsigned*)name);return arena+0xa00;}
extern "C" void __fastcall Close(void* self){record(2,self,0);at(0x20)=0;if(mode==1)at(0xa14)=24;}
extern "C" void __fastcall Position(void* self,void*,unsigned n){if(self!=arena+0x200)exit(22);at(0x204)=n;record(3,self,n);if(mode==2&&n==128)at(4)=0;}
extern "C" void __fastcall Parse(void* result,void* stream){if(stream!=arena+0x200)exit(23);record(4,stream,at(0x204));memcpy(result,arena+0xb00,92);at(0x204)+=92;}
extern "C" void* __fastcall Async(void* self,void*,void* result,unsigned index){if(self!=arena+0x300)exit(24);record(5,self,index);*(unsigned*)result=at(0x800);((unsigned*)result)[1]=at(0x804);if(at(0x804))++*(unsigned*)at(0x804);return result;}
extern "C" void __fastcall SetEntry(void* self,void*,CCountedPointer<CBankFileAsyncEntry> value){record(6,self,(unsigned)value.Data);if(mode==3)at(4)=0;}
extern "C" void __fastcall Land(void* self,void*,void* block,void* stream){if(block!=arena+0x44||stream!=arena+0x200)exit(25);record(7,self,at(0x204));if(mode==4)at(4)=(unsigned)(arena+0x780);}
extern "C" void __fastcall Detail(void* self,void*,void* block,void* stream){if(block!=arena+0x44||stream!=arena+0x200)exit(26);record(8,self,at(0x204));}
extern "C" void __fastcall Release(void* p){record(9,p,0);if(mode==5)at(0x40)=(unsigned)(arena+0x880);if(mode==6)at(4)=0;}
void __cdecl operator delete(void* p){record(10,p,0);}
extern "C" void __cdecl Delete(void*);
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<9;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0x42b467:v=(unsigned)Find;break;case 0xb3ef40:v=(unsigned)Close;break;case 0xb3efa0:v=(unsigned)Parse;break;case 0x9d58d0:v=(unsigned)Async;break;case 0xbfe9bc:v=(unsigned)Delete;break;case 0xbe03a0:v=(unsigned)SetEntry;break;case 0xbdd0e0:v=(unsigned)Land;break;case 0xbdf010:v=(unsigned)Detail;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:OpenRetail;
 unsigned p,cases=0;while(fread(&p,4,1,in)==1){if(fread(arena,1,4096,in)!=4096)return 5;mode=p;events=0;at(0x200)=(unsigned)(arena+0x500);at(0x504)=(unsigned)Position;for(unsigned j=0x840;j<=0x880;j+=0x20){at(j+4)=(unsigned)Release;at(j+8)=(unsigned)(arena+j+12);}
 fn(arena,0,arena+0x300,arena+0x200,arena+0x400);record(11,arena,0);++cases;}
 fclose(in);fclose(output);printf("NATIVE_RETAIL_MAP_OPEN_PASS cases=%u\n",cases);return 0;
}
