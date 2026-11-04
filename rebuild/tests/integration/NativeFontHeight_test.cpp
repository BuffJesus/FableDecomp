#include <stdio.h>
#include <string.h>
#include <float.h>
#include "fable_font_instances.h"
#include <new>
#include <stdlib.h>
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
#ifdef STREAMING_FONT
#define Bank CStreamingFontBank
CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank*,unsigned long){}
#pragma comment(linker,"/alternatename:@HeightCandidate@8=?GetFontMaxHeight@CStreamingFontBank@@UBEMXZ")
#else
#define Bank CStaticFontBank
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
#pragma comment(linker,"/alternatename:@HeightCandidate@8=?GetFontMaxHeight@CStaticFontBank@@UBEMXZ")
#endif
extern "C" float __fastcall HeightCandidate(const Bank*,void*);
typedef float(__fastcall* HeightCall)(const Bank*,void*);
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FONT_HEIGHT_FAILURE line=%d value=%08x mode=%u precision=%u\n",__LINE__,value,mode,precision);return 80;}}while(0)
static unsigned Expected(unsigned value,unsigned mode){
 if(!value)return 0;unsigned exponent=0,t=value;while(t>>=1)++exponent;
 unsigned significand,discarded=0,shift=exponent>23?exponent-23:0;
 if(shift){significand=value>>shift;discarded=value&((1u<<shift)-1);unsigned half=1u<<(shift-1);
  if((mode==0 && (discarded>half || (discarded==half && (significand&1)))) || (mode==2 && discarded))++significand;
  if(significand==0x1000000){significand>>=1;++exponent;}
 }else significand=value<<(23-exponent);
 return ((exponent+127)<<23)|(significand&0x7fffff);
}
// Native base/name dependencies are linked separately. Concrete construction, glyph services and destruction are controlled boundaries.
fable_i32 g_CCharStringInstanceCount_013BD800=0;
Bank::~Bank(){}
C2DExtentsF Bank::GetStringExtents(const wchar_t*,bool)const{abort();return C2DExtentsF();}
void Bank::PrepareToRender(bool)const{abort();}
void Bank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{abort();}
#ifdef STREAMING_FONT
bool Bank::Preload(const CWideString&,bool)const{abort();return false;}
#endif
static unsigned Bits(float number){unsigned bits;memcpy(&bits,&number,4);return bits;}
int main(int argc,char** argv){
 unsigned value=0,mode=0,precision=0;CHECK(argc==1 || argc==2);unsigned char* code=0;HeightCall original=0;
 static const float unsignedBias=4294967296.0f;
 if(argc==2){unsigned char bytes[19];FILE* input=fopen(argv[1],"rb");CHECK(input && fread(bytes,1,19,input)==19 && fgetc(input)==EOF);CHECK(!fclose(input));code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);memcpy(code,bytes,19);*(const float**)(code+13)=&unsignedBias;original=reinterpret_cast<HeightCall>(code);}
 __declspec(align(16)) unsigned char storage[sizeof(Bank)+32],before[sizeof(Bank)];
 memset(storage,0xcd,sizeof(storage));Bank* bank=new(storage+16) Bank(0,0);CHECK(g_CCharStringInstanceCount_013BD800==1);
 unsigned control=_controlfp(0,0),cases=0,originalCases=0;unsigned rounds[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP},precisions[]={_PC_24,_PC_53,_PC_64};
 const unsigned boundaries[]={0,1,2,3,20,255,256,65535,65536,0x7fffff,0x800000,0xffffff,0x1000000,0x1000001,0x1000002,0x1000003,0x1ffffff,0x2000000,0x2000001,0x3fffffff,0x40000000,0x7fffff7f,0x7fffff80,0x7fffff81,0x7fffffff,0x80000000,0x80000001,0x80000080,0xffffff00,0xffffff7f,0xffffff80,0xffffffff};
 for(precision=0;precision<3;++precision)for(mode=0;mode<4;++mode){
  _controlfp(rounds[mode]|precisions[precision],_MCW_RC|_MCW_PC);unsigned expectedControl=_controlfp(0,0)&(_MCW_RC|_MCW_PC),random=0x98765432;
  for(unsigned which=0;which<256;++which){random=random*1664525+1013904223;value=which<32?boundaries[which]:random;
   bank->FontHeight=value;bank->MaxHeight=0x13579bdf;memcpy(before,bank,sizeof(Bank));
   volatile float direct=bank->GetFontMaxHeight();volatile float alias=HeightCandidate(bank,0);const CFontBank* base=bank;volatile float viaBase=base->GetFontMaxHeight();unsigned expected=Expected(value,mode);
   CHECK(Bits(direct)==expected && Bits(alias)==expected && Bits(viaBase)==expected);
   if(original){volatile float native=original(bank,0);CHECK(Bits(native)==expected);++originalCases;}
   CHECK(!memcmp(before,bank,sizeof(Bank)) && (_controlfp(0,0)&(_MCW_RC|_MCW_PC))==expectedControl);
   for(unsigned n=0;n<16;++n)CHECK(storage[n]==0xcd && storage[sizeof(Bank)+16+n]==0xcd);++cases;
  }
 }
 _controlfp(control,_MCW_RC|_MCW_PC);CHECK(cases==3072);bank->~Bank();CHECK(!g_CCharStringInstanceCount_013BD800);if(code)CHECK(VirtualFree(code,0,0x8000));
 printf("NATIVE_FONT_HEIGHT_PASS cases=%u original=%u rounds=4 precisions=3 unchanged_receiver=1\n",cases,originalCases);return 0;
}
