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
extern "C" Handle* __fastcall NativeCopy(Handle*,Handle*,Handle*,const _STL::__false_type&);
extern "C" Handle* __fastcall NativeFill(Handle*,unsigned,const Handle&,const _STL::__false_type&);
extern "C" void __fastcall NativeDestroy(Handle*,Handle*,const _STL::__false_type&);
#pragma comment(linker,"/alternatename:@NativeCopy@16=??$__uninitialized_copy@PAV?$CCountedPointer@VCShaderResource@@@@PAV1@@_STL@@YIPAV?$CCountedPointer@VCShaderResource@@@@PAV1@00ABU__false_type@0@@Z")
#pragma comment(linker,"/alternatename:@NativeFill@16=??$__uninitialized_fill_n@PAV?$CCountedPointer@VCShaderResource@@@@IV1@@_STL@@YIPAV?$CCountedPointer@VCShaderResource@@@@PAV1@IABV1@ABU__false_type@0@@Z")
#pragma comment(linker,"/alternatename:@NativeDestroy@12=??$__destroy_aux@PAV?$CCountedPointer@VCShaderResource@@@@@_STL@@YIXPAV?$CCountedPointer@VCShaderResource@@@@0ABU__false_type@0@@Z")
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


static Handle* changingHandle;
static CCPPointerInfo* replacementInfo;
static unsigned destroyMode,deleteCallbacks;
static void __fastcall DestroyCallback(void* data){
 ++deleteCallbacks;check(data==reinterpret_cast<void*>(0x123456));
 if(destroyMode==1){changingHandle->Info=replacementInfo;allocatedInfo=replacementInfo;}
 if(destroyMode==2){changingHandle->Info=0;allocatedInfo=replacementInfo;}
}
static unsigned destroyCases(){unsigned cases=0;const unsigned refs[]={0,1,2,0xffffffff};
 for(unsigned r=0;r<4;++r)for(destroyMode=0;destroyMode<3;++destroyMode){
  Handle h;changingHandle=&h;CCPPointerInfo* original=static_cast<CCPPointerInfo*>(malloc(12));replacementInfo=static_cast<CCPPointerInfo*>(malloc(12));
  original->RefCount=refs[r];original->DeleteFunc=DestroyCallback;original->Data=reinterpret_cast<void*>(0x123456);replacementInfo->RefCount=77;
  h.Data=reinterpret_cast<CShaderResource*>(0x654321);h.Info=original;allocatedInfo=original;allocatedResource=0;infoDeletes=deleteCallbacks=0;
  NativeDestroy(&h,&h+1,_STL::__false_type());
  check(!h.Data && !h.Info && deleteCallbacks==(refs[r]==1));
  if(refs[r]==1){check(infoDeletes==(destroyMode!=2));if(destroyMode!=0)free(original);if(destroyMode!=1)free(replacementInfo);}
  else {check(!infoDeletes && original->RefCount==refs[r]-1);free(original);free(replacementInfo);}
  ++cases;
 }
 Handle empty;NativeDestroy(&empty,&empty+1,_STL::__false_type());check(!empty.Data && !empty.Info);++cases;
 NativeDestroy(&empty,&empty,_STL::__false_type());++cases;allocatedInfo=0;
 return cases;
}

static unsigned copyFillCases(){unsigned cases=0;const unsigned refs[]={0,1,0xffffffff};
 for(unsigned fill=0;fill<2;++fill)for(unsigned n=0;n<6;++n)for(unsigned r=0;r<3;++r)for(unsigned nullInfo=0;nullInfo<2;++nullInfo){
  CCPPointerInfo block(0,0);block.RefCount=refs[r];Handle inputs[6];unsigned long output[12];memset(output,0xa5,sizeof(output));Handle* dest=reinterpret_cast<Handle*>(output);
  for(unsigned i=0;i<6;++i){inputs[i].Data=reinterpret_cast<CShaderResource*>(0x234500+i*16);inputs[i].Info=nullInfo?0:&block;}
  Handle* result=fill?NativeFill(dest,n,inputs[0],_STL::__false_type()):NativeCopy(inputs,inputs+n,dest,_STL::__false_type());
  check(result==dest+n && block.RefCount==refs[r]+(nullInfo?0:n));
  for(i=0;i<n;++i)check(dest[i].Data==inputs[fill?0:i].Data && dest[i].Info==inputs[0].Info);
  for(i=n*2;i<12;++i)check(output[i]==0xa5a5a5a5);
  for(i=0;i<6;++i)inputs[i].Info=0;++cases;
 }
 Handle input;CCPPointerInfo block(0,0);input.Info=&block;
 check(NativeCopy(&input,&input+1,static_cast<Handle*>(0),_STL::__false_type())==reinterpret_cast<Handle*>(8));
 check(NativeFill(static_cast<Handle*>(0),1U,input,_STL::__false_type())==reinterpret_cast<Handle*>(8));
 check(block.RefCount==1);input.Info=0;return cases+2;
}
static unsigned vectorCases(){unsigned cases=0;
 for(unsigned used=0;used<5;++used)for(unsigned pos=0;pos<=used;++pos)for(unsigned n=0;n<6;++n)for(unsigned shared=0;shared<3;++shared)for(unsigned alias=0;alias<2;++alias){
  if(alias && !used)continue;
  unsigned long vectorWords[3]={0,0,0};ResourceVector& v=*reinterpret_cast<ResourceVector*>(vectorWords);CCPPointerInfo blocks[6]={CCPPointerInfo(0,0),CCPPointerInfo(0,0),CCPPointerInfo(0,0),CCPPointerInfo(0,0),CCPPointerInfo(0,0),CCPPointerInfo(0,0)};
  unsigned expectedRefs[6];for(unsigned b=0;b<6;++b)blocks[b].RefCount=expectedRefs[b]=100;
  Handle input;input.Data=reinterpret_cast<CShaderResource*>(0x345600);input.Info=shared?&blocks[5]:0;
  Handle* old=used?static_cast<Handle*>(malloc(used*8)):0;
  unsigned long expectedData[12];CCPPointerInfo* expectedInfo[12];
  for(unsigned i=0;i<used;++i){old[i].Data=reinterpret_cast<CShaderResource*>(0x456700+i*16);old[i].Info=shared?&blocks[shared==1?0:i]:0;}
  v._M_start=old;v._M_finish=old?old+used:0;v._M_end_of_storage._M_data=v._M_finish;
  const Handle& x=alias?old[used/2]:input;
  unsigned k=0;for(i=0;i<pos;++i){expectedData[k]=(unsigned long)old[i].Data;expectedInfo[k++]=old[i].Info;}
  for(i=0;i<n;++i){expectedData[k]=(unsigned long)x.Data;expectedInfo[k++]=x.Info;if(x.Info)++expectedRefs[x.Info-blocks];}
  for(i=pos;i<used;++i){expectedData[k]=(unsigned long)old[i].Data;expectedInfo[k++]=old[i].Info;}
  v._M_insert_overflow(old?old+pos:0,x,_STL::__false_type(),n,pos==used);
  check(v.size()==used+n && v.capacity()==used+(used>n?used:n));
  for(i=0;i<k;++i)check((unsigned long)v[i].Data==expectedData[i] && v[i].Info==expectedInfo[i]);
  for(b=0;b<6;++b)check(blocks[b].RefCount==expectedRefs[b]);
  free(v._M_start);input.Info=0;++cases;
 }
 return cases;
}

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
 cases+=vectorCases()+destroyCases()+copyFillCases();
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("SHADER_RESOURCE_VECTOR_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;}
