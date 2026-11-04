#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palette_load_aliases.h"
#include "palette_load_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Load(void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events,names,found;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>50)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){record(1,self,n);if(n!=1&&n!=4)exit(21);memcpy(out,(void*)at(0x20c),n);at(0x20c)+=n;at(0x204)+=n;at(0x214)=32;if(mode==6)at(0x10)=0x76543210;}
extern "C" void __fastcall Resize(void* self,void*,unsigned n){if(n>4)exit(22);record(2,self,n);at(4)=(unsigned)(arena+0x500);at(8)=at(4)+n*8;at(12)=at(4)+32;if(mode==5)arena[0x14]=0x80;}
extern "C" void* __fastcall ReadName(void* self,void*,void* result){unsigned value=0x1100+names++;*(unsigned*)result=value;record(3,self,value);if(mode==2)at(4)=(unsigned)(arena+0x600);return result;}
extern "C" void* __fastcall Assign(void* self,void*,const void* other){record(4,self,*(unsigned*)other);*(unsigned*)self=*(unsigned*)other;return self;}
extern "C" void __fastcall Destroy(void* self){record(5,0,*(unsigned*)self);if(mode==3){at(0)=(unsigned)(arena+0x440);at(0x10)=0x87654321;}}
extern "C" unsigned __fastcall Find(void* self,void*,const void* name){record(6,self,*(unsigned*)name);if(mode==4)at(4)=(unsigned)(arena+0x600);return found;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<9;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x993ca0:v=(unsigned)Overflow;break;case 0xbf97a0:v=(unsigned)Resize;break;case 0x99efb0:v=(unsigned)Assign;break;case 0x99eae0:v=(unsigned)Destroy;break;case 0x9ccdc0:v=(unsigned)Find;break;default:return 8;}
 *(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:Load;
 unsigned p[2],cases=0;while(fread(p,4,2,in)==2){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];found=p[1];events=names=0;at(0x200)=(unsigned)(arena+0x700);at(0x718)=(unsigned)ReadName;
 fn(arena,0,arena+0x200);record(7,0,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_TEXTURE_PALETTE_LOAD_PASS cases=%u\n",cases);return 0;
}
