#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_instances.h"
#include "fable_system_manager_state.h"
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned row,systemCalls;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"FONT_VIRTUAL_EXTENTS_FAILURE row=%u line=%d\n",row,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
fable_i32 g_CCharStringInstanceCount_013BD800=0,g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
double g_FableVirtualWidth_013961E8=640.0,g_FableVirtualHeight_013961F0=480.0;
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)];
static CSystemManager* borrowedSystem=reinterpret_cast<CSystemManager*>(systemStorage);
static CDisplayManager* borrowedDisplay=reinterpret_cast<CDisplayManager*>(displayStorage);
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){++systemCalls;return borrowedSystem;}
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
CStaticFontBank::~CStaticFontBank(){}
void CStaticFontBank::PrepareToRender(bool)const{CHECK(false);}
void CStaticFontBank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{CHECK(false);}
extern "C" const wchar_t* __fastcall WideChars(const CWideString*,void*);
#pragma comment(linker,"/alternatename:@WideChars@8=?GetAsCharArray@CWideString@@QBEPBGXZ")
static void Relative(unsigned char* code,unsigned at,void* target){*(unsigned long*)(code+at)=(unsigned long)target-((unsigned long)code+at+4);}
static unsigned char* Original(const char* name,unsigned size){
 FILE* input=fopen(name,"rb");unsigned char bytes[76];CHECK(input && fread(bytes,1,size,input)==size && fgetc(input)==EOF);CHECK(!fclose(input));unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);memcpy(code,bytes,size);Relative(code,16,(void*)WideChars);Relative(code,37,(void*)&GFAXToVX);Relative(code,51,(void*)&GFAYToVY);return code;
}
int main(int argc,char** argv){
 CHECK(argc==1 || argc==3);typedef C2DExtentsF(CFontBank::*ExtentMethod)(const CWideString&)const;typedef float(CFontBank::*WidthMethod)(const CWideString&)const;
 unsigned char* extentCode=0;unsigned char* widthCode=0;ExtentMethod originalExtents=0;WidthMethod originalWidth=0;
 if(argc==3){extentCode=Original(argv[1],76);widthCode=Original(argv[2],69);CHECK(sizeof(originalExtents)==4 && sizeof(originalWidth)==4);memcpy(&originalExtents,&extentCode,4);memcpy(&originalWidth,&widthCode,4);}
 unsigned saved=_controlfp(0,0);_controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);unsigned originalCases=0;
 memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));borrowedSystem->PDisplayManager.Data=borrowedDisplay;
 {CStaticFontBank font(0,0);CGlyph glyphs[64];for(unsigned i=0;i<64;++i){memset(&glyphs[i],0,sizeof(CGlyph));glyphs[i].Offset=i==10?0:1;glyphs[i].Advance=i==10?0:short(2+i);}
 for(unsigned i=0;i<1024;++i){font.GlyphBanks[i].FirstGlyph=0;font.GlyphBanks[i].NoGlyphs=64;font.GlyphBanks[i].Glyphs=glyphs;}font.MinChar=0;font.MaxChar=65535;
 const wchar_t* texts[]={L"",L"A",L"ABC",L"A\nB",L"\n",L"J\nAJ",L"\xfffe\xffff",L"native_31"};
 const unsigned screens[][2]={{640,480},{1280,960},{1920,1080},{320,200}};const double virtuals[][2]={{640,480},{1.25,2.5},{0,0},{1024,768}};const unsigned heights[]={0,12,255,0xffffffff};
 for(unsigned screen=0;screen<4;++screen)for(unsigned v=0;v<4;++v)for(unsigned h=0;h<4;++h)for(unsigned textIndex=0;textIndex<8;++textIndex){
  font.FontHeight=heights[h];borrowedDisplay->RenderTargetDimensionsStorage_[0]=screens[screen][0];borrowedDisplay->RenderTargetDimensionsStorage_[1]=screens[screen][1];g_FableVirtualWidth_013961E8=virtuals[v][0];g_FableVirtualHeight_013961F0=virtuals[v][1];
  unsigned length=0;while(texts[textIndex][length])++length;__declspec(align(16)) unsigned char dataBytes[sizeof(CWideStringData)];CWideStringData* data=reinterpret_cast<CWideStringData*>(dataBytes);data->SetText_(const_cast<wchar_t*>(texts[textIndex]));data->SetFinish_((size_t)(texts[textIndex]+length));data->SetEndOfStorage_((size_t)(texts[textIndex]+length+1));data->owners=2;
  {CWideString text;if(length)*reinterpret_cast<CWideStringData**>(&text)=data;unsigned char before[sizeof(font)],textBefore[sizeof(text)],dataBefore[sizeof(dataBytes)];memcpy(before,&font,sizeof(font));memcpy(textBefore,&text,sizeof(text));memcpy(dataBefore,dataBytes,sizeof(dataBytes));
   C2DExtentsF pixels=font.GetStringExtents(texts[textIndex],false);volatile double denominatorX=screens[screen][0],denominatorY=screens[screen][1];volatile float expectedX=(float)((g_FableVirtualWidth_013961E8*pixels.X)/denominatorX);volatile float yProduct=(float)(g_FableVirtualHeight_013961F0*pixels.Y);volatile float expectedY=(float)((double)yProduct/denominatorY);
   unsigned calls=systemCalls;C2DExtentsF extents=font.GetVStringExtents(text);if(!(systemCalls==calls+2 && extents.X==expectedX && extents.Y==expectedY))fprintf(stderr,"WRAPPER_VALUES pixel=%.9g,%.9g actual=%.9g,%.9g model=%.9g,%.9g calls=%u delta=%u\n",pixels.X,pixels.Y,extents.X,extents.Y,expectedX,expectedY,systemCalls,systemCalls-calls);CHECK(systemCalls==calls+2 && extents.X==expectedX && extents.Y==expectedY);
   calls=systemCalls;volatile float width=font.GetVStringWidth(text);CHECK(systemCalls==calls+2 && width==expectedX);
   if(originalExtents){calls=systemCalls;C2DExtentsF native=(font.*originalExtents)(text);CHECK(systemCalls==calls+2 && !memcmp(&extents,&native,8));calls=systemCalls;volatile float nativeWidth=(font.*originalWidth)(text);CHECK(systemCalls==calls+2 && nativeWidth==width);++originalCases;}
   CHECK(!memcmp(before,&font,sizeof(font)) && !memcmp(textBefore,&text,sizeof(text)) && !memcmp(dataBefore,dataBytes,sizeof(dataBytes)));++row;
  }
  CHECK(g_CWideStringInstanceCount_013BCA20==0 && (!length || data->owners==1));
 }
 }
 CHECK(row==512 && !g_CCharStringInstanceCount_013BD800);if(extentCode)CHECK(VirtualFree(extentCode,0,0x8000));if(widthCode)CHECK(VirtualFree(widthCode,0,0x8000));_controlfp(saved,_MCW_PC|_MCW_RC);
 printf("NATIVE_FONT_VIRTUAL_EXTENTS_PASS cases=%u original=%u coordinate_calls=%u pc53_nearest=1\n",row,originalCases,systemCalls);return 0;
}
