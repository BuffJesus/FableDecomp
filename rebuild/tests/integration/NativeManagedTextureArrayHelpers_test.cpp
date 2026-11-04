#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_array_helpers_retail.h"
#include "managed_array_helpers_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall CopyRoot(void*,void*,void*,const void*,int*);
extern "C" void __fastcall DestroyRoot(void*,void*,const void*);
static unsigned arena[4096],initial[4096],expected[8][4096],captures[8][4096],captured,mutation,base,first,last,result,table[2];
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void capture(){require(captured<8);memcpy(captures[captured++],arena,sizeof(arena));}
extern "C" void __fastcall DeleteOwner(void* self,void*){unsigned log=base+0x1800,n=word(log,0)++;require(n<6);word(log,4+n*4)=(unsigned)self;capture();if(mutation&1)word(first,0)=base+0xb00;if(mutation&2)word(result,0)=base+0xc00;if(mutation&4){word(base+0xb00,4)=0xffffffff;word(base+0xc00,4)=0;}}
static unsigned pattern(unsigned n){const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static unsigned copy_model(){unsigned source=first,out=result;for(int n=(int)(last-first)/4;n>0;--n){unsigned old=word(out,0);if(old!=word(source,0)){if(old){--word(old,4);if(!word(old,4))DeleteOwner((void*)old,0);word(out,0)=0;}unsigned p=word(source,0);word(out,0)=p;if(p)++word(p,4);}source+=4;out+=4;}return out;}
static void destroy_model(){unsigned p=first;while(p!=last){unsigned owner=word(p,0);if(owner){--word(owner,4);if(!word(owner,4))DeleteOwner((void*)owner,0);word(p,0)=0;}p+=4;}}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));table[0]=0;table[1]=(unsigned)DeleteOwner;const int deltas[]={0,1,2,-1,-2,16};unsigned cases=0,snapshots=0;
 for(unsigned operation=0;operation<2;++operation)for(unsigned length=0;length<5;++length)for(unsigned overlap=0;overlap<6;++overlap)for(unsigned ref=0;ref<8;++ref)for(unsigned shape=0;shape<4;++shape)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));base=(unsigned)arena+(place?0x1800:0);first=base+0x440;last=first+length*4;result=first+deltas[overlap]*4;for(unsigned i=0;i<40;++i)word(base+0x400,i*4)=shape==0?0:shape==1?base+0x900:((i+shape)%5==0?0:base+0x900+((i+shape)&3)*0x100);for(unsigned i=0;i<4;++i){word(base+0x900+i*0x100,0)=(unsigned)table;word(base+0x900+i*0x100,4)=pattern(ref+i*(shape==3));}memset((void*)(base+0x1800),0,64);memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0,expectedReturn=0;
  for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;unsigned returned=0;if(!mode){if(operation)destroy_model();else returned=copy_model();expectedReturn=returned;}else if(operation)(mode==1?((void(__fastcall*)(void*,void*,const void*))(code+128)):DestroyRoot)((void*)first,(void*)last,(void*)(base+0x1400));else returned=(unsigned)(mode==1?((void*(__fastcall*)(void*,void*,void*,const void*,int*))code):CopyRoot)((void*)first,(void*)last,(void*)result,(void*)(base+0x1400),0);if(mode)require(returned==expectedReturn);word(base+0x1800,56)=returned;capture();if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_ARRAY_HELPERS_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
