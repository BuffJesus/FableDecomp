#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_bank.h"
#include "fable_virtual_pixels.h"
#include "font_vstring_width_retail.h"
#include "font_vstring_width_aliases.h"
class CSystemManager;
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" float __fastcall FontWidth(void*,void*,const void*);
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
static unsigned memory[8192],initial[8192],model[8192],snapshots[3][8192];static double expectedGlobals[3][2];
static unsigned count,mutation,pattern,position;static unsigned char* font;static const wchar_t* wantedText;
static unsigned extentsX,extentsY;static void require(bool b){if(!b){fprintf(stderr,"require count=%u pattern=%u\n",count,pattern);fflush(stderr);abort();}}
static float number(unsigned value){float f;memcpy(&f,&value,4);return f;}
static unsigned bits(float value){unsigned n;memcpy(&n,&value,4);return n;}
static bool nan(unsigned n){return (n&0x7fffffff)>0x7f800000;}
static void capture(){require(count<3);require(!memcmp(memory,snapshots[count],sizeof(memory)));require(g_FableVirtualWidth_013961E8==expectedGlobals[count][0]&&g_FableVirtualHeight_013961F0==expectedGlobals[count][1]);++count;}
class MeasureBoundary{public:C2DExtentsF Measure(const wchar_t* text,bool flag) const;};
C2DExtentsF MeasureBoundary::Measure(const wchar_t* text,bool flag)const{
 require(this==(void*)font&&text==wantedText&&!flag);if(mutation){*(unsigned*)(font+8)^=0x12345678;*(unsigned*)(font+0x100)=0;}capture();return C2DExtentsF(number(extentsX),number(extentsY));
}
#pragma comment(linker,"/alternatename:@MeasureEntry@8=?Measure@MeasureBoundary@@QBE?AVC2DExtentsF@@PBG_N@Z")
extern "C" void __fastcall MeasureEntry(void*,void*);
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){
 require(count==1||count==2);unsigned char* a=(unsigned char*)memory;
 if(mutation){*(void**)(a+0x160)=a+(count==1?0x2000:0x1000);g_FableVirtualWidth_013961E8=(count==1?1000.125:2000.25)+pattern;g_FableVirtualHeight_013961F0=(count==1?800.375:1200.5)-pattern;}
 capture();return (CSystemManager*)(a+0x100);
}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(fixups)/sizeof(fixups[0]);++i){const Fixup& f=fixups[i];unsigned target=f.target==0xffffffff?(unsigned)GFGetSystemManager:f.target==0xfffffffe?(unsigned)&g_FableVirtualWidth_013961E8:f.target==0xfffffffd?(unsigned)&g_FableVirtualHeight_013961F0:f.target==0xfffffffc?(unsigned)g_FableEmptyWideString_0129A8E0:(unsigned)(code+f.target);*(unsigned*)(code+f.site)=f.kind==6?target:target-(unsigned)(code+f.site+4);}
 unsigned table[6]={0,(unsigned)MeasureEntry,0,0,0,0};unsigned cases=0,oldControl=_control87(0,0);const int dimensions[8]={128,640,1024,-256,0,0x7fffffff,1,-1};const unsigned values[16]={0,0x80000000,0x3f800000,0x3faaaaab,0x4640e600,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,1,0x80000001,0x00800000,0xc2800000,0x3e800000,0x3dcccccd};
 for(unsigned precision=0;precision<3;++precision){_control87(precision==0?_PC_24:precision==1?_PC_53:_PC_64,_MCW_PC);
 for(pattern=0;pattern<128;++pattern)for(unsigned stringState=0;stringState<3;++stringState)for(mutation=0;mutation<2;++mutation)for(position=0;position<2;++position)for(unsigned fill=0;fill<2;++fill){
  memset(initial,fill?0xa9:0,sizeof(initial));unsigned char* a=(unsigned char*)initial;unsigned foff=position?0x4000:0x3000;unsigned char* f=a+foff;*(unsigned*)f=(unsigned)table;
  *(unsigned*)(f+0x100)=stringState?(unsigned)((unsigned char*)memory+foff+0x200):0;*(unsigned*)(f+0x200)=(unsigned)((unsigned char*)memory+foff+0x300);*(unsigned short*)(f+0x300)=stringState==2?0:'A';*(unsigned short*)(f+0x302)=0;
  *(unsigned*)(a+0x160)=(unsigned)((unsigned char*)memory+0x1000);*(int*)(a+0x1194)=dimensions[pattern%8];*(int*)(a+0x1198)=dimensions[(pattern+3)%8];*(int*)(a+0x2194)=dimensions[(pattern+5)%8];*(int*)(a+0x2198)=dimensions[(pattern+6)%8];
  extentsX=values[pattern/8];extentsY=values[(pattern/8+3)%16];double vw=65536,vh=65536;memcpy(model,initial,sizeof(model));unsigned char* m=(unsigned char*)model;
  if(mutation){*(unsigned*)(m+foff+8)^=0x12345678;*(unsigned*)(m+foff+0x100)=0;}memcpy(snapshots[0],model,sizeof(model));expectedGlobals[0][0]=vw;expectedGlobals[0][1]=vh;
  if(mutation){*(unsigned*)(m+0x160)=(unsigned)((unsigned char*)memory+0x2000);vw=1000.125+pattern;vh=800.375-pattern;}memcpy(snapshots[1],model,sizeof(model));expectedGlobals[1][0]=vw;expectedGlobals[1][1]=vh;
  unsigned expectedBits=bits((float)((vw*(double)number(extentsX))/dimensions[(pattern+(mutation?5:0))%8]));
  if(mutation){*(unsigned*)(m+0x160)=(unsigned)((unsigned char*)memory+0x1000);vw=2000.25+pattern;vh=1200.5-pattern;}memcpy(snapshots[2],model,sizeof(model));expectedGlobals[2][0]=vw;expectedGlobals[2][1]=vh;
  unsigned results[2];for(unsigned run=0;run<2;++run){memcpy(memory,initial,sizeof(memory));font=(unsigned char*)memory+foff;wantedText=stringState?(const wchar_t*)(font+0x300):g_FableEmptyWideString_0129A8E0;g_FableVirtualWidth_013961E8=g_FableVirtualHeight_013961F0=65536;count=0;_clear87();results[run]=bits(run?FontWidth(font,0,font+0x100):((float(__fastcall*)(void*,void*,const void*))code)(font,0,font+0x100));require(count==3&&!memcmp(memory,model,sizeof(memory)));require(nan(expectedBits)?nan(results[run]):results[run]==expectedBits);}
  require(results[0]==results[1]);
  double firstHeight=expectedGlobals[1][1];volatile float numerator=(float)(firstHeight*(double)number(extentsY));unsigned wantedY=bits((float)((double)numerator/dimensions[(pattern+(mutation?6:3))%8]));
  for(unsigned run=0;run<2;++run){memcpy(memory,snapshots[0],sizeof(memory));g_FableVirtualWidth_013961E8=expectedGlobals[0][0];g_FableVirtualHeight_013961F0=expectedGlobals[0][1];count=1;_clear87();results[run]=bits(run?GFAYToVY(number(extentsY)):((float(__fastcall*)(float))(code+768))(number(extentsY)));require(count==2&&!memcmp(memory,snapshots[1],sizeof(memory)));require(nan(wantedY)?nan(results[run]):results[run]==wantedY);}
  require(results[0]==results[1]);++cases;
 }
 }
 _clear87();_control87(oldControl,_MCW_PC);VirtualFree(code,0,0x8000);printf("NATIVE_FONT_VSTRING_WIDTH_PASS cases=%u snapshots=%u width_cases=%u vertical_cases=%u\n",cases*2,cases*12,cases,cases);return 0;
}
