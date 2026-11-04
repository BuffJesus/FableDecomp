#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <float.h>
#include "fable_font_instances.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned row;
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FONT_EXTENTS_FAILURE row=%u line=%d\n",row,__LINE__);return 80;}}while(0)
fable_i32 g_CCharStringInstanceCount_013BD800=0;
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
CStaticFontBank::~CStaticFontBank(){}
void CStaticFontBank::PrepareToRender(bool)const{abort();}
void CStaticFontBank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{abort();}
static C2DExtentsF Expected(const CStaticFontBank& font,const wchar_t* text,bool stop){
 volatile float width=0,height=static_cast<float>(font.FontHeight+1);const float lineHeight=height;
 float maxWidth=0,maxHeight=0;
 for(unsigned i=0;text[i];++i){unsigned c=text[i];
  if(c==10){if(stop)break;width=0;height=height+lineHeight;}
  if((long)c<font.MinChar || (long)c>font.MaxChar)continue;
  const CStaticFontGlyphBank& bank=font.GlyphBanks[c/64];unsigned n=(c%64)-bank.FirstGlyph;
  if(bank.Glyphs && n<bank.NoGlyphs)width=bank.Glyphs[n].Offset+width+bank.Glyphs[n].Advance;
  if(width>maxWidth)maxWidth=width;if(height>maxHeight)maxHeight=height;
 }
 return C2DExtentsF(maxWidth,maxHeight);
}
int main(int argc,char** argv){
 CHECK(argc==1 || argc==2);unsigned char* code=0;
 typedef C2DExtentsF(CStaticFontBank::*Method)(const wchar_t*,bool)const;Method original=0;
 if(argc==2){FILE* input=fopen(argv[1],"rb");unsigned char bytes[287];CHECK(input && fread(bytes,1,287,input)==287 && fgetc(input)==EOF);CHECK(!fclose(input));
 code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);memcpy(code,bytes,287);static const float zero=0.0f,bias=4294967296.0f;
 *(const float**)(code+8)=&zero;*(const float**)(code+47)=&bias;CHECK(sizeof(original)==sizeof(code));memcpy(&original,&code,sizeof(code));}
 unsigned savedControl=_controlfp(0,0);_controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);unsigned originalCases=0;
 {CStaticFontBank font(0,0);CGlyph glyphs[64];unsigned char before[sizeof(font)],glyphBefore[sizeof(glyphs)];
 const unsigned heights[]={0,1,12,64,0x7fffffff,0xfffffffe,0xffffffff};
 const wchar_t* texts[]={L"",L"A",L"ABC",L"\n",L"\nA",L"A\n",L"A\nB",L"A\n\nB",L"\x0001",L"?@AB",L"\x007f\x0080",L"\xfffe\xffff",L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",L"0123456789",L"A\nB\nC",L"\x003b\x003c\x003d\x003f\x0040\x007f\x0080\xfffe"};
 const long lows[]={0,32,65,70000,-7,0};const long highs[]={65535,128,90,70010,65535,-1};
 for(unsigned h=0;h<7;++h)for(unsigned bounds=0;bounds<6;++bounds)for(unsigned config=0;config<4;++config){
  font.FontHeight=heights[h];font.MinChar=lows[bounds];font.MaxChar=highs[bounds];
  for(unsigned i=0;i<64;++i){memset(&glyphs[i],0xa5,sizeof(glyphs[i]));glyphs[i].Offset=(short)((int(i)*311%401)-200);glyphs[i].Advance=(short)((int(i)*173%601)-250);glyphs[i].Width=(short)0x7fed;if(i==0 || i==60){glyphs[i].Offset=7;glyphs[i].Advance=11;}}
  for(unsigned i=0;i<1024;++i){font.GlyphBanks[i].FirstGlyph=(unsigned short)(config==1?4:0);font.GlyphBanks[i].NoGlyphs=(unsigned short)(config==2?0:60);font.GlyphBanks[i].Glyphs=(config==3 && (i&1))?0:glyphs;}
  for(unsigned t=0;t<16;++t)for(unsigned stop=0;stop<2;++stop){
   memcpy(before,&font,sizeof(font));memcpy(glyphBefore,glyphs,sizeof(glyphs));
   C2DExtentsF candidate=font.GetStringExtents(texts[t],stop!=0),expected=Expected(font,texts[t],stop!=0);
   CHECK(!memcmp(&candidate,&expected,8));
   if(original){C2DExtentsF native=(font.*original)(texts[t],stop!=0);CHECK(!memcmp(&candidate,&native,8));++originalCases;}
   CHECK(!memcmp(before,&font,sizeof(font)) && !memcmp(glyphBefore,glyphs,sizeof(glyphs)) && g_CCharStringInstanceCount_013BD800==1);++row;
  }
 }
 // A long run crosses the float mantissa boundary and exposes retained x87 width.
 font.FontHeight=12;font.MinChar=0;font.MaxChar=65535;
 font.GlyphBanks[1].FirstGlyph=0;font.GlyphBanks[1].NoGlyphs=64;font.GlyphBanks[1].Glyphs=glyphs;
 glyphs[1].Offset=32767;glyphs[1].Advance=32767;
 wchar_t longText[2049];for(unsigned i=0;i<2048;++i)longText[i]=L'A';longText[2048]=0;
 C2DExtentsF candidate=font.GetStringExtents(longText,false),expected=Expected(font,longText,false);
 CHECK(!memcmp(&candidate,&expected,8));if(original){C2DExtentsF native=(font.*original)(longText,false);CHECK(!memcmp(&candidate,&native,8));++originalCases;}++row;
 }
 CHECK(row==5377 && !g_CCharStringInstanceCount_013BD800);if(code)CHECK(VirtualFree(code,0,0x8000));_controlfp(savedControl,_MCW_PC|_MCW_RC);printf("NATIVE_STATIC_FONT_EXTENTS_PASS cases=%u original=%u model=%u pc53_nearest=1\n",row,originalCases,row);return 0;
}
