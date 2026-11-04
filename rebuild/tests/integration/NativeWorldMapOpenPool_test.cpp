#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world_open_pool_aliases.h"
#include "world_open_pool_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall OpenMaps(void*,void*,unsigned);
extern "C" { unsigned Resources,Water,SeaName,CommonName; }
static unsigned char* arena;static FILE* output;static unsigned mode,events,opened,index;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>100)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Grid(void* self){record(1,self,0);if(mode==1){at(0x20)=(unsigned)(arena+0x500);at(0x24)=at(0x20)+12;}}
extern "C" unsigned __fastcall BankOpen(void* self,void*,const void* name,unsigned flags){if(name!=arena+0x30||flags!=0x62)exit(21);record(2,self,flags);if(mode==2)at(0xbe4)=(unsigned)(arena+0xd10);return opened;}
extern "C" void __fastcall Pool(void*,void*,void*,void*);
extern "C" void __fastcall Release(void* data){record(14,data,0);if(mode==3)at(0x150)=(unsigned)(arena+0xd40);if(mode==7)at(0xd00)=9;}
void __cdecl operator delete(void* p){record(15,p,0);}
extern "C" void __cdecl Delete(void*);
extern "C" void __fastcall Text(void* self,void*,const char* str,long len){if(strcmp(str,"SetStaticMapFileForUse: OpenStaticMap")||len!=-1)exit(22);*(unsigned*)self=0x11223344;record(5,0,0);}
extern "C" void __fastcall Copy(void* self,void*,const void* src){if(src!=&SeaName)exit(23);*(unsigned*)self=*(unsigned*)src;record(6,0,*(unsigned*)src);}
extern "C" void __fastcall Destroy(void* self){record(7,0,*(unsigned*)self);if(mode==4){at(0x20)=(unsigned)(arena+0x500);at(0x24)=at(0x20)+12;}}
extern "C" void __fastcall Progress(void*,unsigned char b,float f,unsigned c){if(b||c||f!=-1.0f)exit(24);record(8,0,0);}
extern "C" void __fastcall MapOpen(void* self,void*,void* bank,unsigned state){if(bank!=arena+0x34)exit(25);record(9,self,state);if(mode==5)at(0x24)=at(0x20);}
extern "C" void __fastcall WaterOpen(void*,void*,void*,unsigned);
extern "C" void* __fastcall Assign(void* self,void*,const void* other){record(10,self,*(unsigned*)other);*(unsigned*)self=*(unsigned*)other;if(mode==6){at(0x20)=(unsigned)(arena+0x500);at(0x24)=at(0x20)+12;}return self;}
extern "C" unsigned __fastcall Find(void* self,void*,void* name){if(name!=&CommonName)exit(27);record(11,self,CommonName);return index;}
extern "C" void __fastcall Common(void* self,void*,unsigned id){record(12,self,id);if(mode==7)at(0x1a8)=55;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0xb40070:v=(unsigned)Grid;break;case 0x99ebf0:v=(unsigned)Text;break;case 0x99eae0:v=(unsigned)Destroy;break;case 0x99ec30:v=(unsigned)Copy;break;case 0x9e9f40:v=(unsigned)Progress;break;case 0xb42530:v=(unsigned)MapOpen;break;case 0xb3e820:v=(unsigned)(code+400);break;case 0xbfe9bc:v=(unsigned)Delete;break;case 0xb6d4d0:v=(unsigned)(code+512);break;case 0x99efb0:v=(unsigned)Assign;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0xb420f0:v=(unsigned)Common;break;
 case 0x1436e98:v=(unsigned)&Resources;break;case 0x1436e54:v=(unsigned)&Water;break;case 0x1436ec4:v=(unsigned)&SeaName;break;case 0x1436ecc:v=(unsigned)&CommonName;break;case 0x12a1f84:v=(unsigned)"SetStaticMapFileForUse: OpenStaticMap";break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;}
 typedef void (__fastcall *Fn)(void*,void*,unsigned);Fn fn=argv[3][0]=='r'?(Fn)code:OpenMaps;
 unsigned p[4],cases=0;while(fread(p,4,4,in)==4){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];opened=p[2];index=p[3];events=0;Resources=(unsigned)(arena+0x300);Water=(unsigned)(arena+0x600);SeaName=0xabc;CommonName=0xdef;at(0x34)=(unsigned)(arena+0xf00);at(0xf0c)=(unsigned)BankOpen;for(unsigned j=0xd00;j<=0xd40;j+=0x10){at(j+4)=(unsigned)Release;at(j+8)=(unsigned)(arena+j+0xc);}
 fn(arena,0,p[1]);record(13,arena,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_WORLD_MAP_OPEN_PASS cases=%u\n",cases);return 0;
}
