// Borrowed display/system and concrete font/glyph lifecycle are fixture boundaries.
// Real loaded strings are measured by native static extents using synthetic glyph metrics.
// Real bank strings, AddText, coordinates, record/texture copies and vector ownership execute.
#include <new>
#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_font_instances.h"
#include "fable_2d_vector.h"
#include "fable_stlport_vector_impl.h"
extern "C" void ColdBoundary();
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static void check(bool ok,int line){if(!ok){fprintf(stderr,"DRAW_QUEUE_FAILURE line=%d\n",line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) check((x)!=0,__LINE__)

// Concrete parsing, glyph upload and render services remain controlled fixture boundaries.
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
CStaticFontBank::~CStaticFontBank(){}
void CStaticFontBank::PrepareToRender(bool)const{ColdBoundary();}
void CStaticFontBank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{ColdBoundary();}
static CGlyph syntheticGlyphs[64];
class BorrowedFont:public CStaticFontBank {
public:
 BorrowedFont():CStaticFontBank(0,0){
  FontHeight=16;PManager=0;MinChar=0;MaxChar=65535;
  for(unsigned i=0;i<64;++i){memset(&syntheticGlyphs[i],0,sizeof(CGlyph));syntheticGlyphs[i].Offset=i==10?0:1;syntheticGlyphs[i].Advance=i==10?0:short(2+i);syntheticGlyphs[i].Width=73;}
  for(unsigned i=0;i<1024;++i){GlyphBanks[i].FirstGlyph=0;GlyphBanks[i].NoGlyphs=64;GlyphBanks[i].Glyphs=syntheticGlyphs;}
 }
};
static unsigned metricChecks;
static void CheckMetrics(const CFontBank& font,const CWideString& text,bool firstLine){
 unsigned width=0,height=17,maxWidth=0,maxHeight=0;const wchar_t* data=(const wchar_t*)text;
 for(unsigned i=0;data[i];++i){unsigned c=data[i];if(c==10){if(firstLine)break;width=0;height+=17;}else if((c&63)!=10)width+=3+(c&63);if(width>maxWidth)maxWidth=width;if(height>maxHeight)maxHeight=height;}
 C2DExtentsF actual=font.GetStringExtents(data,firstLine);CHECK(actual.X==float(maxWidth) && actual.Y==float(maxHeight) && font.GetFontMaxHeight()==16.0f);++metricChecks;
}
__declspec(align(16)) static unsigned char fontStorage[sizeof(BorrowedFont)];
static BorrowedFont* font;
__declspec(align(16)) static unsigned char rendererStorage[sizeof(CRenderManager2D)];
__declspec(align(16)) static unsigned char displayStorage[sizeof(CDisplayManager)];
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)];
static CRenderManager2D* renderer;static CDisplayManager* display;static CSystemManager* borrowedSystem;static unsigned queued;
double g_FableVirtualWidth_013961E8=640.0,g_FableVirtualHeight_013961F0=480.0;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){CHECK(borrowedSystem && renderer && display);return borrowedSystem;}
struct CRenderManager2DTestAccess {
 static void Add(const CWideString& text,unsigned index){C3DVector point;point.X=100.0f+index;point.Y=50.0f+index;point.Z=0.25f;CRGBColour colour(0x12,0x34,0x56,0x78);renderer->AddText(point,1.25f,text,*font,colour,0x12340000+index);}
};
static void Initialise(){
 CHECK(!font);font=new(fontStorage) BorrowedFont;
 {CCharString narrow("J\nAJ");CWideString wide=narrow.ToWideString();CheckMetrics(*font,wide,false);CheckMetrics(*font,wide,true);CHECK(metricChecks==2);metricChecks=0;}
 memset(rendererStorage,0,sizeof(rendererStorage));memset(displayStorage,0,sizeof(displayStorage));memset(systemStorage,0,sizeof(systemStorage));renderer=reinterpret_cast<CRenderManager2D*>(rendererStorage);display=reinterpret_cast<CDisplayManager*>(displayStorage);borrowedSystem=reinterpret_cast<CSystemManager*>(systemStorage);
 renderer->DisplayManager=display;borrowedSystem->PDisplayManager.Data=display;display->RenderTargetDimensionsStorage_[0]=1280;display->RenderTargetDimensionsStorage_[1]=960;display->ViewportBoxFloat=C2DBoxF(0,0,1280,960);new(&renderer->QuickDraw2DTriInfo) _STL::vector<CQuickDrawTriInfo>;
}
extern "C" void QueueTextFromBank(const CWideString& text,unsigned index){
 if(!renderer)Initialise();CHECK(index==queued);CheckMetrics(*font,text,false);CheckMetrics(*font,text,true);CHECK(font->Preload(text,false) && font->Preload(text,true));display->ViewportOffscreen=true;CRenderManager2DTestAccess::Add(text,index);CHECK(renderer->QuickDraw2DTriInfo.size()==queued);display->ViewportOffscreen=false;CRenderManager2DTestAccess::Add(text,index);CHECK(renderer->QuickDraw2DTriInfo.size()==queued+1);
 const CQuickDrawTriInfo& value=renderer->QuickDraw2DTriInfo.back();CHECK(value.IsText && !value.Line && value.PFont==font && !value.UploadedTexture.PD3DTexture && value.StringFlags==0x12340000+index && value.StringScale==1.25f && value.StringX==2*(100.0f+index) && value.StringY==2*(50.0f+index) && value.StringZ==0.25f);
 CHECK(value.Window.left==0 && value.Window.top==0 && value.Window.right==1280 && value.Window.bottom==960 && value.Colour.R==0x12 && value.Colour.G==0x34 && value.Colour.B==0x56 && value.Colour.A==0x78);
 CHECK(value.String.GetLength()==text.GetLength() && !memcmp((const wchar_t*)value.String,(const wchar_t*)text,(text.GetLength()+1)*2));++queued;
}
extern "C" void CheckQueuedBankText(unsigned index,const unsigned char* expected,unsigned bytes){CHECK(renderer && index<queued);const CWideString& value=renderer->QuickDraw2DTriInfo[index].String;CheckMetrics(*font,value,false);CheckMetrics(*font,value,true);CHECK(value.GetLength()*2==bytes && !memcmp((const wchar_t*)value,expected,bytes+2));}
extern "C" void ClearQueuedBankTexts(unsigned expected){CHECK(queued==expected && renderer->QuickDraw2DTriInfo.size()==expected && metricChecks==expected*4);metricChecks=0;renderer->QuickDraw2DTriInfo.~vector();font->~BorrowedFont();font=0;renderer=0;display=0;borrowedSystem=0;queued=0;printf("NATIVE_LOADED_TEXT_DRAW_QUEUE_PASS records=%u offscreen_refusals=%u after_resource_release=%u cleanup=1 synthetic_metrics=1 rasterization=0\n",expected,expected,expected);}
