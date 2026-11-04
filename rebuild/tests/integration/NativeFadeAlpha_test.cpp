#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_fade_in_out.h"
#include "fade_alpha_retail.h"
#pragma comment(linker,"/alternatename:@FadeAlpha@8=?GetFadeInOutAlpha@CFadeInFadeOutBase@@QAEMXZ")
extern "C" float __fastcall FadeAlpha(void*,void*);
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static void require(bool b){if(!b){fprintf(stderr,"require failed\n");fflush(stderr);abort();}}
static const unsigned values[16]={0,0x80000000,0x38d1b716,0x38d1b717,0x38d1b718,0x3f800000,0x40000000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x80000001,0x7f7fffff,0x00800000};
static float number(unsigned bits){float value;memcpy(&value,&bits,4);return value;}
static unsigned bits(float value){unsigned out;memcpy(&out,&value,4);return out;}
static bool nan(unsigned v){return (v&0x7fffffff)>0x7f800000;}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,96,0x3000,0x40);require(code!=0);memcpy(code,retailCode,79);float constants[3]={0.0001f,1.0f,0.0f};
 for(unsigned i=0;i<sizeof(constantSites)/sizeof(constantSites[0]);++i)*(float**)(code+constantSites[i][0])=&constants[constantSites[i][1]];
 unsigned arena[2048],before[2048];unsigned cases=0;unsigned oldControl=_control87(0,0);
 setvbuf(stderr,0,_IONBF,0);
 for(unsigned precision=0;precision<3;++precision){_control87(precision==0?_PC_24:precision==1?_PC_53:_PC_64,_MCW_PC);
 for(unsigned flags=0;flags<8;++flags)for(unsigned di=0;di<16;++di)for(unsigned ni=0;ni<16;++ni)for(unsigned pos=0;pos<2;++pos)for(unsigned fill=0;fill<2;++fill){
  memset(arena,fill?0x95:0,sizeof(arena));unsigned char* p=(unsigned char*)arena+(pos?0x200:0);p[0]=(flags&1)?1:0;p[0xc]=(flags&2)?1:0;p[0xd]=(flags&4)?1:0;
  *(unsigned*)(p+4)=values[di];*(unsigned*)(p+8)=values[(di+5)%16];*(unsigned*)(p+0x10)=values[ni];*(unsigned*)(p+0x14)=values[(ni+3)%16];memcpy(before,arena,sizeof(arena));
  double expected=(flags&1)?1.0:0.0;
  if(flags&6){unsigned db=(flags&4)?values[di]:values[(di+5)%16];unsigned nb=(flags&4)?values[ni]:values[(ni+3)%16];double duration=number(db);expected=duration<(double)constants[0]?1.0:(double)number(nb)/duration;}
  unsigned expectedBits=bits((float)expected);
  unsigned result[2];for(unsigned run=0;run<2;++run){_clear87();float value=run?FadeAlpha(p,0):((float(__fastcall*)(void*,void*))code)(p,0);result[run]=bits(value);if(memcmp(arena,before,sizeof(arena))){fprintf(stderr,"memory precision=%u flags=%u di=%u ni=%u run=%u\n",precision,flags,di,ni,run);fflush(stderr);abort();}if(!(nan(expectedBits)?nan(result[run]):result[run]==expectedBits)){fprintf(stderr,"model precision=%u flags=%u di=%u ni=%u run=%u value=%08x expected=%08x\n",precision,flags,di,ni,run,result[run],expectedBits);fflush(stderr);abort();}}
  if(result[0]!=result[1]){fprintf(stderr,"pair precision=%u flags=%u di=%u ni=%u native=%08x built=%08x\n",precision,flags,di,ni,result[0],result[1]);fflush(stderr);abort();}++cases;
 }
 }
 _clear87();_control87(oldControl,_MCW_PC);VirtualFree(code,0,0x8000);printf("NATIVE_FADE_ALPHA_PASS cases=%u snapshots=%u\n",cases,cases*2);return 0;
}
