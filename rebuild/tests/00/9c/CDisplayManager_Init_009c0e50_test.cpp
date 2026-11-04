#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_system.h"
#include "fable_shader_render_manager.h"
#include "fable_exception.h"
#include "../../integration/DisplayRendererBoundaries.h"
#include "../../integration/ShaderManagerFixtureLifetime.h"
#pragma comment(linker,"/alternatename:?Uninitialise@CTexture@@QAEXXZ=_UnusedDisplayRendererBoundary")
#pragma comment(linker,"/alternatename:??1CWideString@@QAE@XZ=_UnusedDisplayRendererBoundary")
static CDisplayManager* display;static CDisplayManagerInit* input;
static int failures,flags,exclusive,capsFail,resultCode,old,mutation;
static int capsCalls,timerCtors,timerDtors,renderCtors,renderDtors,renderInits,shaderInits,virtualCalls,resizeCalls;
static void* api[1];static void* apiVtable[17];static void* oldTimer;static CRenderManager* oldRender;
static void check(bool b){if(!b)++failures;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CRenderStateManager::~CRenderStateManager(){}
CRenderStateManager::CDeviceState::CDeviceState(){}
CRenderStateManager::CRenderStateManager(unsigned long){}
CInitBaseClass::CInitBaseClass():Valid(false){}
CInitBaseClass::~CInitBaseClass(){}
void CInitBaseClass::Validate() const {}
CRenderManagerCore::CRenderManagerCore(CDisplayManager& owner,unsigned long flags):CRenderStateManager(flags),DisplayManager(&owner){}
CRenderManagerCore::~CRenderManagerCore(){++renderDtors;check(this==oldRender && display->PRenderManager.get()!=this);}
CRenderManager2D::CRenderManager2D(CDisplayManager& owner,const CWideString& path,unsigned long initFlags):CRenderManagerCore(owner,initFlags){
 ++renderCtors;check(&owner==display && &path==&display->ShaderResourcePath && initFlags==input->RenderManagerInitFlags);
}
void CRenderManagerCore::Init(IDirect3DDevice9* device){++renderInits;check(this==display->PRenderManager.get() && device==display->D3DDevice);if(mutation==5)display->UsingShaderRenderManager=true;}
CShaderRenderManager FableShaderRenderManager_013bc470;
void CShaderRenderManager::Initialise(CRenderManager* renderer,IDirect3DDevice9* device){++shaderInits;check(this==&FableShaderRenderManager_013bc470 && renderer==display->PRenderManager.get() && device==display->D3DDevice);}
CWideString& CWideString::operator=(const CWideString& other){check(this==&display->ShaderResourcePath && &other==&input->ShaderResourcePath);if(mutation==3)input->RenderManagerInitFlags=0x2222;return *this;}
namespace _STL {template<> list<CVsyncCallbackTimer::CPendingQuery>::~list(){free(_M_node);}}
CVsyncCallbackTimer::CVsyncCallbackTimer(){++timerCtors;}
CVsyncCallbackTimer::~CVsyncCallbackTimer(){++timerDtors;check(this==oldTimer && display->VsyncCallbackTimer.get()!=this);}
extern "C" IDirect3D9* FABLE_STDCALL Direct3DCreate9(unsigned int version){check(version==32);return reinterpret_cast<IDirect3D9*>(api);}
static HRESULT FABLE_STDCALL Caps(void* self,unsigned int adapter,D3DDEVTYPE type,D3DCAPS9* out){
 ++capsCalls;check(self==api && adapter==0 && type==capsCalls && out==&display->D3DDeviceCaps);
 memset(out,capsCalls,sizeof(*out));if(mutation==1){display->WindowHandle=reinterpret_cast<HWND__*>(0x87654321);input->Width=31;input->Height=33;display->ExclusiveMode=!exclusive;}
 return capsFail?static_cast<HRESULT>(0x80004005):0;
}
CDisplayManager::ECreateDeviceSuccess CDisplayManager::CreateDevice(const CCreateDeviceParameters& p){
 check(this==display && p.WinHandle==WindowHandle && p.BackBufferDimensions.X==(input->Width<32?32:input->Width) && p.BackBufferDimensions.Y==(input->Height<32?32:input->Height));
 check(p.BackBufferBitDepth==32 && p.ZBufferBitDepth==24 && p.BackBufferCount==2 && p.RefreshRate==75 && p.ExclusiveMode==bool(exclusive));
 check(p.PresentationInterval.PresentImmediate==bool(flags&1) && p.PresentationInterval.PresentImmediateIfSyncMissed==bool(flags&2) && p.PresentationInterval.VSyncCount==2 && p.MultisampleType==4);
 D3DDevice=reinterpret_cast<IDirect3DDevice9*>(0x345600);return static_cast<ECreateDeviceSuccess>(resultCode);
}
void CDisplayManager::ResizeWindowToBackBufferSize(){++resizeCalls;if(mutation==2){input->RenderManagerInitFlags=0x1111;UsingRenderManager=true;}}
void CDisplayManager::InitVirtualPixels(){++virtualCalls;check(this==display);}
int main(){
 api[0]=apiVtable;apiVtable[14]=reinterpret_cast<void*>(&Caps);unsigned scenarios=0;
 const long dimensions[][2]={{-1,0},{31,32},{33,720},{1920,1080}};
 for(int d=0;d<4;++d)for(flags=0;flags<4;++flags)for(exclusive=0;exclusive<2;++exclusive)for(capsFail=0;capsFail<2;++capsFail)for(resultCode=0;resultCode<3;++resultCode)for(old=0;old<2;++old)for(mutation=0;mutation<6;++mutation){
  unsigned long object[0x2c8/4],initStorage[0x3c/4];memset(object,0xa5,sizeof(object));memset(initStorage,0,sizeof(initStorage));display=reinterpret_cast<CDisplayManager*>(object);input=reinterpret_cast<CDisplayManagerInit*>(initStorage);
  input->Width=dimensions[d][0];input->Height=dimensions[d][1];input->Depth=32;input->RefreshRate=75;input->UseRenderManager=(flags&1)!=0;input->UseShaderRenderManager=(flags&2)!=0;input->WindowHandle=reinterpret_cast<void*>(0x12345678);input->ZBufferDepth=24;input->NoBackBuffers=2;input->MultisampleType=4;input->PresentImmediate=(flags&1)!=0;input->PresentImmediatelyIfMissedVSync=(flags&2)!=0;input->PresentationIntevalVSyncCount=2;input->RenderManagerInitFlags=0x5678;
  oldTimer=old?new CVsyncCallbackTimer:0;oldRender=old?new CRenderManager(*display,display->ShaderResourcePath,input->RenderManagerInitFlags):0;
  new(&display->VsyncCallbackTimer) boost::scoped_ptr<CVsyncCallbackTimer>(static_cast<CVsyncCallbackTimer*>(oldTimer));new(&display->PRenderManager) boost::scoped_ptr<CRenderManager>(oldRender);
  capsCalls=timerCtors=timerDtors=renderCtors=renderDtors=renderInits=shaderInits=virtualCalls=resizeCalls=0;
  bool thrown=false;try{display->Init(*input,exclusive!=0);}catch(const CGenericException&){thrown=true;}catch(...){check(false);}
  bool render=(flags&1)!=0 || mutation==2;
  check(thrown==(resultCode!=2) && timerCtors==1 && timerDtors==old && capsCalls==(capsFail?2:1));
  check(resizeCalls==(resultCode==2) && virtualCalls==(resultCode==2));
  check(renderCtors==((resultCode==2 && render)?1:0) && renderInits==renderCtors && renderDtors==((resultCode==2 && render && old)?1:0));
  check(shaderInits==((resultCode==2 && render && ((flags==3)||mutation==5))?1:0));
  check(!display->DeviceLostFlag && !display->CanBeginRender && display->MultisampleType==0);
  if(resultCode==2)check(display->RenderManagerInitFlags==(mutation==2?0x1111:0x5678));
  // Raw fixture objects deliberately have no lifetime cleanup; release owned
  // allocations directly after observations without invoking extra callbacks.
  CVsyncCallbackTimer* timer=display->VsyncCallbackTimer.get();unsigned long* words=reinterpret_cast<unsigned long*>(timer);free(reinterpret_cast<void*>(words[2]));free(reinterpret_cast<void*>(words[1]));::operator delete(timer);
  ::operator delete(display->PRenderManager.get());++scenarios;
 }
 printf("DISPLAY_INIT_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}

