#include "fable_string.h"
#include <stdio.h>
#include <stdlib.h>
#include "texture_palette_aliases.h"
#include "texture_palette_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Construct(void*,void*,void*,unsigned,unsigned);
extern "C" void __fastcall Grow(void*,void*,void*,const void*,const void*,unsigned,unsigned);
extern "C" void __fastcall DefaultText(void*);
extern "C" void __fastcall CopyText(void*,void*,const void*);
extern "C" void __fastcall DestroyText(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static void record(unsigned kind,void* self,unsigned arg){if(++events>60)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
CCharString::CCharString(){record(1,0,0);*(unsigned*)this=0x1111;if(mode==1||mode==2){*(void**)(arena+4)=arena+0x200;*(void**)(arena+8)=arena+0x200;*(void**)(arena+12)=arena+(mode==1?0x218:0x200);}}
CCharString::CCharString(const CCharString& other){record(2,this,*(unsigned*)&other);*(unsigned*)this=*(unsigned*)&other;if(mode==3)*(unsigned*)(arena+0x204)+=1;}
CCharString::~CCharString(){record(3,(unsigned char*)this>=arena&&(unsigned char*)this<arena+4096?this:0,*(unsigned*)this);if(mode==4)arena[20]=0x80;}
extern "C" void* __cdecl PaletteAllocate(unsigned n){if(n>64)exit(10);record(4,0,n);if(mode==5)*(unsigned*)(arena+0x204)+=7;return arena+0x800;}
extern "C" void __cdecl PaletteFree(void* p){record(5,p,0);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0x99e4b0:v=(unsigned)DefaultText;break;case 0x99ec30:v=(unsigned)CopyText;break;case 0x99eae0:v=(unsigned)DestroyText;break;case 0xbfea0e:v=(unsigned)PaletteAllocate;break;case 0xbfea14:v=(unsigned)PaletteFree;break;case 0xbf94d0:v=(unsigned)(code+192);break;case 0xbf8f30:v=(unsigned)(code+448);break;case 0xbf8f70:v=(unsigned)(code+512);break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void* (__fastcall *Ctor)(void*,void*,void*,unsigned,unsigned);typedef void (__fastcall *Insert)(void*,void*,void*,const void*,const void*,unsigned,unsigned);
 Ctor ctor=argv[3][0]=='r'?(Ctor)code:Construct;Insert grow=argv[3][0]=='r'?(Insert)(code+192):Grow;
 unsigned p[6],cases=0;while(fread(p,4,6,in)==6){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];events=0;
  if(p[1]==0){if(ctor(arena,0,(void*)p[2],p[3],p[4])!=arena)return 6;}else grow(arena+4,0,p[2]?arena+p[2]:0,arena+0x500,arena+0x600,p[3],p[4]);record(6,0,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_TEXTURE_PALETTE_PASS cases=%u\n",cases);return 0;
}
