// Borrowed display/borrowedSystem context and fail-fast font service are fixture boundaries.
// Real bank strings, AddText, coordinates, record/texture copies and vector ownership execute.
#include <new>
#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_font_bank.h"
#include "fable_2d_vector.h"
#include "fable_stlport_vector_impl.h"
extern "C" void ColdBoundary();
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static void check(bool ok,int line){if(!ok){fprintf(stderr,"DRAW_QUEUE_FAILURE line=%d\n",line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) check((x)!=0,__LINE__)

class BorrowedFont:public CFontBank {
public:
 BorrowedFont(){FontHeight=0;PManager=0;}
 virtual C2DExtentsF GetStringExtents(const wchar_t*,bool) const{ColdBoundary();return C2DExtentsF();}
 virtual float GetFontMaxHeight() const{ColdBoundary();return 0;}
 virtual void PrepareToRender(bool) const{ColdBoundary();}
 virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*) const{ColdBoundary();}
};
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
 memset(rendererStorage,0,sizeof(rendererStorage));memset(displayStorage,0,sizeof(displayStorage));memset(systemStorage,0,sizeof(systemStorage));renderer=reinterpret_cast<CRenderManager2D*>(rendererStorage);display=reinterpret_cast<CDisplayManager*>(displayStorage);borrowedSystem=reinterpret_cast<CSystemManager*>(systemStorage);
 renderer->DisplayManager=display;borrowedSystem->PDisplayManager.Data=display;display->RenderTargetDimensionsStorage_[0]=1280;display->RenderTargetDimensionsStorage_[1]=960;display->ViewportBoxFloat=C2DBoxF(0,0,1280,960);new(&renderer->QuickDraw2DTriInfo) _STL::vector<CQuickDrawTriInfo>;
}
extern "C" void QueueTextFromBank(const CWideString& text,unsigned index){
 if(!renderer)Initialise();CHECK(index==queued);CHECK(font->Preload(text,false) && font->Preload(text,true));display->ViewportOffscreen=true;CRenderManager2DTestAccess::Add(text,index);CHECK(renderer->QuickDraw2DTriInfo.size()==queued);display->ViewportOffscreen=false;CRenderManager2DTestAccess::Add(text,index);CHECK(renderer->QuickDraw2DTriInfo.size()==queued+1);
 const CQuickDrawTriInfo& value=renderer->QuickDraw2DTriInfo.back();CHECK(value.IsText && !value.Line && value.PFont==font && !value.UploadedTexture.PD3DTexture && value.StringFlags==0x12340000+index && value.StringScale==1.25f && value.StringX==2*(100.0f+index) && value.StringY==2*(50.0f+index) && value.StringZ==0.25f);
 CHECK(value.Window.left==0 && value.Window.top==0 && value.Window.right==1280 && value.Window.bottom==960 && value.Colour.R==0x12 && value.Colour.G==0x34 && value.Colour.B==0x56 && value.Colour.A==0x78);
 CHECK(value.String.GetLength()==text.GetLength() && !memcmp((const wchar_t*)value.String,(const wchar_t*)text,(text.GetLength()+1)*2));++queued;
}
extern "C" void CheckQueuedBankText(unsigned index,const unsigned char* expected,unsigned bytes){CHECK(renderer && index<queued);const CWideString& value=renderer->QuickDraw2DTriInfo[index].String;CHECK(value.GetLength()*2==bytes && !memcmp((const wchar_t*)value,expected,bytes+2));}
extern "C" void ClearQueuedBankTexts(unsigned expected){CHECK(queued==expected && renderer->QuickDraw2DTriInfo.size()==expected);renderer->QuickDraw2DTriInfo.~vector();font->~BorrowedFont();font=0;renderer=0;display=0;borrowedSystem=0;queued=0;printf("NATIVE_LOADED_TEXT_DRAW_QUEUE_PASS records=%u offscreen_refusals=%u after_resource_release=%u cleanup=1 rasterization=0\n",expected,expected,expected);}
