#include <stdio.h>
#include <string.h>
#include "fable_font_bank.h"
// Unchanged native string helpers are dependencies; the candidate is linked separately.
#include "../../../src/compiled/00/99/CWideString_Constructor_0099aed0.cpp"
#include "../../../src/compiled/00/99/CWideString_Destructor_0099b510.cpp"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
fable_i32 g_CWideStringInstanceCount_013BCA20=0;
// Actual native dependencies: actual native font base and empty owned Name lifetime; glyph services remain fixture boundaries.
fable_i32 g_CCharStringInstanceCount_013BD800=0;
#include "../../../src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp"
#include "../../../src/compiled/00/99/CCharString_DefaultConstructor_0099e4b0.cpp"
#include "../../../src/compiled/00/99/CCharString_Destructor_0099eae0.cpp"
#include "../../../src/compiled/00/99/CCharString_UnassignString_0099e9b0.cpp"


class GuardFont:public CFontBank {
public:
 GuardFont(){FontHeight=0;PManager=0;}
 virtual C2DExtentsF GetStringExtents(const wchar_t*,bool)const{return C2DExtentsF();}
 virtual float GetFontMaxHeight()const{return 0;}
 virtual void PrepareToRender(bool)const{}
 virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{}
};
extern "C" bool __fastcall FontPreloadCandidate(const CFontBank*,void*,const CWideString&,bool);
#pragma comment(linker,"/alternatename:@FontPreloadCandidate@16=?Preload@CFontBank@@UBE_NABVCWideString@@_N@Z")
typedef bool (__fastcall* PreloadCall)(const CFontBank*,void*,const CWideString&,bool);
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FONT_PRELOAD_FAILURE line=%d\n",__LINE__);return 80;}}while(0)
int main(int argc,char** argv){
 CHECK(argc==1 || argc==2);void* code=0;PreloadCall original=0;
 if(argc==2){unsigned char bytes[5];FILE* input=fopen(argv[1],"rb");CHECK(input && fread(bytes,1,5,input)==5 && fgetc(input)==EOF);CHECK(!fclose(input));code=VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);memcpy(code,bytes,5);original=reinterpret_cast<PreloadCall>(code);}
 unsigned cases=0,originalCases=0;
 {GuardFont font;CWideString strings[2];CHECK(g_CWideStringInstanceCount_013BCA20==2 && g_CCharStringInstanceCount_013BD800==1);
  for(unsigned pattern=0;pattern<64;++pattern)for(unsigned wait=0;wait<2;++wait)for(unsigned which=0;which<2;++which){
   font.FontHeight=pattern*0x01010101;unsigned char before[sizeof(font)],textBefore[sizeof(strings)];memcpy(before,&font,sizeof(font));memcpy(textBefore,strings,sizeof(strings));
   CHECK(FontPreloadCandidate(&font,0,strings[which],wait!=0));CHECK(font.Preload(strings[which],wait!=0));
   if(original){CHECK(original(&font,0,strings[which],wait!=0));++originalCases;}
   CHECK(!memcmp(before,&font,sizeof(font)) && !memcmp(textBefore,strings,sizeof(strings)) && g_CWideStringInstanceCount_013BCA20==2);++cases;
  }
 }
 CHECK(cases==256 && !g_CWideStringInstanceCount_013BCA20 && !g_CCharStringInstanceCount_013BD800);if(code)CHECK(VirtualFree(code,0,0x8000));
 printf("NATIVE_FONT_PRELOAD_BEHAVIOR_PASS cases=%u original=%u\n",cases,originalCases);return 0;
}
