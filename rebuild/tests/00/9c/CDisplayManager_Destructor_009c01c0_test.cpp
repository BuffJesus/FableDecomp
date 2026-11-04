#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "../../integration/DisplayRendererBoundaries.h"
#include "../../integration/ShaderManagerFixtureLifetime.h"
static CDisplayManager* display;static CRenderManager* replacement;
static int failures,mutation,events[40],eventCount,mask;
static void* device[1];static void* deviceTable[3];static CVsyncCallbackTimer* timer;
static void check(bool b){if(!b)++failures;}
static void event(int value){check(eventCount<40);events[eventCount++]=value;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){if(this==static_cast<CBaseClassNonCopyable*>(display))event(99);}
CRenderStateManager::~CRenderStateManager(){}
CRenderStateManager::CDeviceState::CDeviceState(){}
CRenderStateManager::CRenderStateManager(unsigned long){}
CInitBaseClass::CInitBaseClass():Valid(false){}
CInitBaseClass::~CInitBaseClass(){}
void CInitBaseClass::Validate() const {}
CRenderManagerCore::CRenderManagerCore(CDisplayManager& owner,unsigned long flags):CRenderStateManager(flags),DisplayManager(&owner){}
CRenderManagerCore::~CRenderManagerCore(){event(this==replacement?11:10);if(mutation==2 && !display->PRenderManager.get())new(&display->PRenderManager) boost::scoped_ptr<CRenderManager>(replacement);}
CRenderManager2D::CRenderManager2D(CDisplayManager& owner,const CWideString&,unsigned long flags):CRenderManagerCore(owner,flags){}
CShaderRenderManager FableShaderRenderManager_013bc470;
void CShaderRenderManager::Uninitialise(){event(1);}
void CSurface::Uninitialise(){int offset=reinterpret_cast<char*>(this)-reinterpret_cast<char*>(display);event(100+offset);if(mutation==3 && offset==0x3c)new(&display->PRenderManager) boost::scoped_ptr<CRenderManager>(replacement);if(mutation==4 && offset==0x2c)display->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(device);PD3DSurface=0;}
void CTexture::Uninitialise(){event(80);}
CWideString::~CWideString(){check(this==&display->ShaderResourcePath);event(70);}
namespace _STL {
template<> list<CBusyResourceManager::CPendingQuery>::~list(){}
template<> list<CVsyncCallbackTimer::CPendingQuery>::~list(){}
}
CBusyResourceManager::~CBusyResourceManager(){event(30);if(mutation==5)new(&display->VsyncCallbackTimer) boost::scoped_ptr<CVsyncCallbackTimer>(timer);}
CVsyncCallbackTimer::~CVsyncCallbackTimer(){check(this==timer);event(40);}
static unsigned long FABLE_STDCALL Release(void* self){check(self==device);event(20);return 0;}
static void initList(_STL::list<CDeviceResetCallback*>& list,int count){
 typedef _STL::list<CDeviceResetCallback*>::_Node Node;
 Node* head=static_cast<Node*>(malloc(sizeof(Node)));head->_M_next=head;head->_M_prev=head;list._M_node=head;
 for(int i=0;i<count;++i){Node* node=static_cast<Node*>(malloc(sizeof(Node)));node->_M_data=reinterpret_cast<CDeviceResetCallback*>(0x123400+i);node->_M_next=head;node->_M_prev=head->_M_prev;head->_M_prev->_M_next=node;head->_M_prev=node;}
}
int main(){unsigned scenarios=0;device[0]=deviceTable;deviceTable[2]=reinterpret_cast<void*>(&Release);
 for(mask=0;mask<64;++mask)for(int count=0;count<16;++count)for(mutation=0;mutation<6;++mutation){
  unsigned long storage[0x2c8/4];memset(storage,0xa5,sizeof(storage));display=reinterpret_cast<CDisplayManager*>(storage);
  CRenderManager* first=(mask&2)?new CRenderManager(*display,display->ShaderResourcePath,0):0;replacement=new CRenderManager(*display,display->ShaderResourcePath,0);
  new(&display->PRenderManager) boost::scoped_ptr<CRenderManager>(first);timer=static_cast<CVsyncCallbackTimer*>(::operator new(sizeof(CVsyncCallbackTimer)));new(&display->VsyncCallbackTimer) boost::scoped_ptr<CVsyncCallbackTimer>((mask&8)?timer:0);
  display->UsingShaderRenderManager=(mask&1)!=0;display->D3DDevice=(mask&4)?reinterpret_cast<IDirect3DDevice9*>(device):0;
  display->BackBufferFormats._M_start=(mask&16)?static_cast<CPixelFormat*>(malloc(16)):0;display->MultisampleTypes._M_start=(mask&32)?static_cast<ESurfaceMultisampleType*>(malloc(16)):0;initList(display->DeviceResetCallbacks,count);eventCount=0;
  display->CDisplayManager::~CDisplayManager();
  int expected[40],n=0;if(mask&1)expected[n++]=1;if(first)expected[n++]=10;expected[n++]=144;expected[n++]=160;if((mask&4)||mutation==4)expected[n++]=20;expected[n++]=30;if((mask&8)||mutation==5)expected[n++]=40;expected[n++]=70;expected[n++]=80;expected[n++]=160;expected[n++]=144;expected[n++]=128;expected[n++]=112;
  bool replaced=(mutation==2 && first)||mutation==3;if(replaced)expected[n++]=11;expected[n++]=99;
  check(eventCount==n && memcmp(events,expected,n*sizeof(int))==0);
  if(!replaced)::operator delete(replacement);if(!(mask&8)&&mutation!=5)::operator delete(timer);++scenarios;
 }
 printf("DISPLAY_DESTRUCTOR_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}

