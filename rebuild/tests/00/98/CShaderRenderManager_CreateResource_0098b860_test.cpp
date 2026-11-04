#include <new>
#include <excpt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_shader_render_manager.h"
#include "fable_shader_resource.h"
extern "C" void __fastcall InvokeShaderDeleteData(void*);
#pragma comment(linker,"/alternatename:@InvokeShaderDeleteData@4=?DeleteData@?$CCountedPointer@VCShaderResource@@@@CIXPAX@Z")
typedef CCountedPointer<CShaderResource> Handle;
typedef _STL::vector<Handle> ResourceVector;
static CShaderRenderManager* manager;
static CShaderResource* allocatedResource;
static CCPPointerInfo* allocatedInfo;
static unsigned failed,allocationMode,callbackMode,allocationCalls,constructCalls,initCalls,overflowCalls,resourceDeletes,infoDeletes;
static bool recording;
static unsigned long resultStorage[2];
static ResourceVector& resources(){return manager->ResourceManagers;}
static void check(bool v){if(!v)++failed;}
void* operator new(size_t n){
 if(recording){++allocationCalls;check((allocationCalls==1 && n==60)||(allocationCalls==2 && n==12));if(allocationMode==allocationCalls)return 0;}
 void* p=malloc(n);
 if(recording){if(n==60)allocatedResource=static_cast<CShaderResource*>(p);else if(n==12)allocatedInfo=static_cast<CCPPointerInfo*>(p);}
 return p;
}
void operator delete(void* p){if(p==allocatedResource)++resourceDeletes;if(p==allocatedInfo)++infoDeletes;free(p);}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CVertexShaderInput::CVertexShaderInput(){}
CShaderResource::CShaderResource():ShaderBank(0),ShaderRenderManager(0),VertexShaderDeclaration(0),VertexFormat(static_cast<EVertexType>(0)){++constructCalls;}
CShaderResource::~CShaderResource(){}
void CShaderResource::Initialise(CShaderRenderManager& value){
 ShaderRenderManager=&value;++initCalls;check(&value==manager);
 if(callbackMode==1)resources()._M_end_of_storage._M_data=resources()._M_finish;
 if(allocatedInfo && callbackMode==2)allocatedInfo->RefCount=0;
 if(allocatedInfo && callbackMode==3)allocatedInfo->RefCount=0xffffffffUL;
}
__declspec(noinline) static void Invoke(){new(resultStorage) Handle(manager->CreateResource());}
static unsigned long Attempt(){__try {Invoke();return 0;}__except(1){return _exception_code();}}
int main(){unsigned cases=0;const unsigned fills[4]={0,1,0x55,0xa5};
 for(unsigned f=0;f<4;++f)for(allocationMode=0;allocationMode<3;++allocationMode)for(unsigned used=0;used<3;++used)for(unsigned spare=0;spare<2;++spare)for(callbackMode=0;callbackMode<4;++callbackMode){
  unsigned long storage[sizeof(CShaderRenderManager)/sizeof(unsigned long)];manager=reinterpret_cast<CShaderRenderManager*>(storage);memset(manager,fills[f],sizeof(storage));
  unsigned cap=used+spare;Handle* initial=cap?static_cast<Handle*>(malloc(cap*8)):0;if(cap)memset(initial,0,cap*8);
  resources()._M_start=initial;resources()._M_finish=initial?initial+used:0;resources()._M_end_of_storage._M_data=initial?initial+cap:0;
  allocatedResource=0;allocatedInfo=0;allocationCalls=constructCalls=initCalls=overflowCalls=resourceDeletes=infoDeletes=0;memset(resultStorage,0x66,sizeof(resultStorage));recording=true;
  unsigned long fault=Attempt();recording=false;
  if(allocationMode==1){check(fault==0xc0000005 && allocationCalls==1 && !constructCalls && !initCalls && !overflowCalls);check(resources()._M_start==initial && resources().size()==used);free(initial);++cases;continue;}
  check(!fault && allocationCalls==2 && constructCalls==1 && initCalls==1);
  Handle* result=reinterpret_cast<Handle*>(resultStorage);check(result->Data==allocatedResource && result->Info==allocatedInfo);
  check(resources().size()==used+1);
  check((resources()._M_start!=initial)==bool(!spare || callbackMode==1));
  check(resources()[used].Data==allocatedResource && resources()[used].Info==allocatedInfo);
  if(allocationMode==2){check(!allocatedInfo && !resourceDeletes && !infoDeletes);delete allocatedResource;result->Data=0;resources()[used].Data=0;}
  else if(callbackMode==3){check(resourceDeletes==1 && infoDeletes==1);result->Info=0;resources()[used].Info=0;}
  else {check(!resourceDeletes && !infoDeletes && allocatedInfo->RefCount==(callbackMode==2?1:2));allocatedInfo->RefCount=2;}
  for(unsigned i=0;i<resources().size();++i)resources()[i].~Handle();free(resources()._M_start);result->~Handle();
  check(resourceDeletes==1 && infoDeletes==(allocationMode==2?0:1));
  ++cases;
 }
 unsigned priorDeletes=resourceDeletes+infoDeletes;InvokeShaderDeleteData(0);check(resourceDeletes+infoDeletes==priorDeletes);++cases;
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("SHADER_RESOURCE_CREATE_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;}
