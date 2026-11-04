#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "gfuninitialise_aliases.h"
#include "gfuninitialise_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u stage %u\n",__LINE__,cases,stage);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __cdecl RunGF();extern "C" unsigned char IME[84];
extern "C" {unsigned char Game[16],Bank[16],Thread[16];}
static unsigned char *arena,expected[1024],ei[84],eg[16],eb[16],et[16];static unsigned p[4],cases,stage,active;static bool running;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,1024)&&!memcmp(IME,ei,84)&&!memcmp(Game,eg,16)&&!memcmp(Bank,eb,16)&&!memcmp(Thread,et,16));fwrite(arena,1,1024,output);fwrite(IME,1,84,output);fwrite(Game,1,16,output);fwrite(Bank,1,16,output);fwrite(Thread,1,16,output);}
static void Step(unsigned value){REQUIRE(stage++==value);++W(arena,value*4);++W(expected,value*4);Record();}
extern "C" void __fastcall GameClose(void*self,void*){REQUIRE(self==Game);++W(Game,0);++W(eg,0);Step(0);}
extern "C" void __fastcall FileRelease(){Step(1);}
extern "C" void __fastcall BankClose(void*self,void*){REQUIRE(self==Bank);++W(Bank,0);++W(eb,0);Step(2);}
extern "C" void* __fastcall Root(){Step(3);return arena+(p[1]&1?0x140:0x100);}
extern "C" void __fastcall SystemClose(void*self,void*){REQUIRE(self==arena+(p[1]&1?0x140:0x100));Step(4);}
extern "C" void __fastcall ThreadClose(void*self,void*){REQUIRE(self==Thread);++W(Thread,0);++W(et,0);Step(5);}
extern "C" void __fastcall FontClose(){if(p[1]&1)IME[8]=ei[8]=!IME[8];active=IME[8]!=0;Step(6);}
extern "C" void __fastcall Finalize(bool send){REQUIRE(!send);if(p[1]&1)W(IME,4)=W(ei,4)=0x12345678;Step(7);}
extern "C" void __fastcall Enable(bool enabled){REQUIRE(!enabled);if(p[1]&2)W(IME,0x1c)=W(ei,0x1c)=(unsigned)(arena+0x300);Step(8);}
extern "C" bool __fastcall Initialize(void*window,bool disable){REQUIRE((unsigned)window==W(ei,4)&&disable);if(p[1]&4){IME[0]=ei[0]=1;IME[8]=ei[8]=1;IME[0x50]^=1;ei[0x50]^=1;}Step(9);return (p[1]&1)!=0;}
void __cdecl operator delete(void*ptr){if(!running){free(ptr);return;}REQUIRE((unsigned)ptr==W(ei,0x1c));ei[8]=0;if(p[1]&4)W(IME,0x4c)=W(ei,0x4c)=(unsigned)(arena+0x340);Step(10);}
static void FinalModel(){if(!active)return;ei[0]=ei[8]=0;W(ei,4)=W(ei,0x4c)=0;memset(ei+12,0,64);W(ei,0x10)=0x00404040;W(ei,0x14)=0xff000000;ei[0x18]=24;ei[0x19]=160;W(ei,0x20)=0xffffffff;W(ei,0x24)=0xff000000;W(ei,0x2c)=0xffff00;W(ei,0x30)=0xff;W(ei,0x34)=0xff00;W(ei,0x38)=W(ei,0x3c)=0xff0000;ei[0x40]=128;ei[0x48]=2;ei[0x49]=1;}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<37;++i){unsigned v,a=retailFixups[i][1];if(a>=0x13b7cd8&&a<0x13b7d2c)v=(unsigned)IME+a-0x13b7cd8;else switch(a){case 0x13b83d0:v=(unsigned)Game;break;case 0x13ca79c:v=(unsigned)Bank;break;case 0x13bc870:v=(unsigned)Thread;break;case 0x412f60:v=(unsigned)&GameClose;break;case 0x406df0:v=(unsigned)&FileRelease;break;case 0x9a7990:v=(unsigned)&BankClose;break;case 0x9a4ec0:v=(unsigned)&Root;break;case 0x9a60d0:v=(unsigned)&SystemClose;break;case 0x98e0d0:v=(unsigned)&ThreadClose;break;case 0x9a1660:v=(unsigned)&FontClose;break;case 0x404a80:v=(unsigned)(code+64);break;case 0x4051c0:v=(unsigned)(code+80);break;case 0x404a90:v=(unsigned)(code+176);break;case 0xcb2c60:v=(unsigned)&Finalize;break;case 0xcb5130:v=(unsigned)&Enable;break;case 0xcb5180:v=(unsigned)&Initialize;break;case 0xbfe9bc:v=(unsigned)(void(__cdecl*)(void*))&operator delete;break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 typedef void(__cdecl*Fn)();Fn fn=!strcmp(argv[3],"retail")?(Fn)code:&RunGF;
 while(fread(p,4,4,in)==4){memset(arena,0x95,1024);memset(IME,p[3]*31,84);IME[8]=(unsigned char)p[0];W(IME,4)=0xabcdef01;W(IME,0x1c)=p[2]?(unsigned)(arena+0x280):0;memset(Game,p[3],16);memset(Bank,p[3]+1,16);memset(Thread,p[3]+2,16);memcpy(expected,arena,1024);memcpy(ei,IME,84);memcpy(eg,Game,16);memcpy(eb,Bank,16);memcpy(et,Thread,16);stage=0;running=true;fn();running=false;REQUIRE(stage==(active?11:7));FinalModel();Record();++cases;}
 fclose(in);fclose(output);printf("NATIVE_GFUNINITIALISE_PASS cases=%u\n",cases);return 0;
}
