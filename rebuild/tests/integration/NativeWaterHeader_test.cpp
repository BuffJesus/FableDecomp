#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "water_header_aliases.h"
#include "water_header_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Header(void*,void*,void*,long);
extern "C" void __fastcall NonPatch(void*,void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,unsigned a,unsigned b){if(++events>8)exit(10);unsigned h[]={kind,a,b};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* dest,long size){
 if(self!=arena+0x200||(size!=4&&size!=8))exit(20);
 unsigned ptr=at(0x20c);if(ptr<(unsigned)arena+0x500||ptr+size>(unsigned)arena+0x540)exit(21);
 memcpy(dest,(void*)ptr,size);at(0x20c)+=size;at(0x204)+=size;at(0x214)=0;
 unsigned d=(unsigned)dest-(unsigned)arena;record(1,d<2048?d:0,size);
 if(mode==1)at(0x204)=0x7ffffffe;
 if(mode==2){at(0x20c)=(unsigned)(arena+0x520);at(0x214)=16;}
 if(mode==3)at(0x204)=0xfffffffc;
}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<4;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0x993ca0:v=(unsigned)Overflow;break;case 0xb6d6e0:v=(unsigned)code;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef bool (__fastcall *HF)(void*,void*,void*,long);typedef void (__fastcall *PF)(void*,void*,void*,void*);
 HF header=argv[3][0]=='r'?(HF)code:Header;PF parent=argv[3][0]=='r'?(PF)(code+256):NonPatch;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[1];events=0;
  if(p[0]){parent(arena+0x300,0,arena+0x400,arena+0x200);record(3,0,0);}
  else {bool result=header(arena+0x100,0,arena+0x200,(long)p[2]);if(result!=(p[2]==8))return 6;record(2,result,0);}++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_WATER_HEADER_PASS cases=%u\n",cases);return 0;
}
