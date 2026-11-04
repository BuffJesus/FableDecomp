#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_child_open_aliases.h"
#include "map_child_open_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Landscape(void*,void*,void*,void*);
extern "C" void __fastcall Detail(void*,void*,void*,void*);
extern "C" unsigned __security_cookie;
extern "C" void __fastcall __security_check_cookie(unsigned);
static unsigned char* arena;static FILE* output;static unsigned mode,events,allocated;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>50)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){record(1,self,n);if(n!=12&&n!=20)exit(21);memcpy(out,(void*)at(0x30c),n);at(0x30c)+=n;at(0x304)+=n;at(0x314)=128;if(mode==1)at(0x204)=(unsigned)(arena+0x640);}
extern "C" void __fastcall Release(void* self){record(2,self,0);if(mode==2)at(0x1c)=(unsigned)(arena+0x640);}
void __cdecl operator delete(void* p){record(3,p,0);}
extern "C" void __cdecl Delete(void*);
extern "C" void* __cdecl Allocate(unsigned n){if(n>64)exit(22);record(4,0,n);allocated=n;if(mode==3)at(0x24)=27;return arena+0x800;}
extern "C" void __cdecl Free(void* p){record(5,p,0);}
extern "C" void __fastcall Position(void* self,void*,unsigned n){for(unsigned i=0;i<allocated;++i)if(arena[0x800+i])exit(23);record(6,self,n);at(0x304)=n;if(mode==4)at(0x20)=(unsigned)(arena+0x280);}
extern "C" void __fastcall Palette(void* self,void*,void* stream){if(stream!=arena+0x300)exit(24);record(7,self,at(0x304));}
extern "C" void __fastcall Quad(void* self,void*,void* stream){if(stream!=arena+0x300)exit(25);record(8,self,at(0x304));if(mode==5)at(0x104)=(unsigned)(arena+0x280);}
extern "C" void __fastcall Types(void* self,void*,void* stream){if(stream!=arena+0x300)exit(26);record(9,self,at(0x304));}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x993ca0:v=(unsigned)Overflow;break;case 0xbfe9bc:v=(unsigned)Delete;break;case 0xbfea0e:v=(unsigned)Allocate;break;case 0xbfea14:v=(unsigned)Free;break;case 0xbf9290:v=(unsigned)Palette;break;case 0xbde290:v=(unsigned)Quad;break;case 0xbdedd0:v=(unsigned)Types;break;case 0x139c8a8:v=(unsigned)&__security_cookie;break;case 0xbfe9f9:v=(unsigned)__security_check_cookie;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;}
 typedef void (__fastcall *Fn)(void*,void*,void*,void*);Fn land=argv[3][0]=='r'?(Fn)code:Landscape,detail=argv[3][0]=='r'?(Fn)(code+320):Detail;
 unsigned p[2],cases=0;while(fread(p,4,2,in)==2){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];events=allocated=0;at(0x300)=(unsigned)(arena+0x400);at(0x404)=(unsigned)Position;for(unsigned j=0x600;j<=0x640;j+=0x20){at(j+4)=(unsigned)Release;at(j+8)=(unsigned)(arena+j+0xc);}
 if(p[1]!=1)land(arena,0,arena+0x200,arena+0x300);if(p[1]!=0)detail(arena+0x100,0,arena+0x200,arena+0x300);record(10,0,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_MAP_CHILD_OPEN_PASS cases=%u\n",cases);return 0;
}
