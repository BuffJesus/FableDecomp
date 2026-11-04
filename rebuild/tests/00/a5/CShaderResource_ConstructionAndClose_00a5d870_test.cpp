#include "fable_shader_resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
struct CShaderResourceTestAccess {
 static CShaderResource* Construct(void* p){return new(p) CShaderResource;}
 static void Uninit(CShaderResource* p){p->Uninitialise();}
 static void Set(CShaderResource* p,CShaderBase** begin,unsigned count){p->Shaders._M_start=begin;p->Shaders._M_finish=begin+count;}
 static CShaderBase** Begin(CShaderResource* p){return p->Shaders._M_start;}
 static void SetBank(CShaderResource* p,CShaderDataBank* bank){p->ShaderBank=bank;}
 static const CShaderDataBank* Bank(CShaderResource* p){return p->ShaderBank;}
 static void SetManager(CShaderResource* p,CShaderRenderManager* manager){p->ShaderRenderManager=manager;}
 static CShaderRenderManager* Manager(CShaderResource* p){return p->ShaderRenderManager;}
};
class ResourceBoundary {public:void BaseConstruct();void InputConstruct();};
#pragma comment(linker,"/alternatename:??0CBaseClassNonCopyable@@QAE@XZ=?BaseConstruct@ResourceBoundary@@QAEXXZ")
#pragma comment(linker,"/alternatename:??0CVertexShaderInput@@QAE@XZ=?InputConstruct@ResourceBoundary@@QAEXXZ")
extern "C" void ResourceUnexpectedCall(){abort();}
#pragma comment(linker,"/alternatename:??1CShaderResource@@UAE@XZ=_ResourceUnexpectedCall")
#pragma comment(linker,"/alternatename:?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ=_ResourceUnexpectedCall")
static unsigned failed,calls,mode,events,expectedCount;
static CShaderResource* owner;
static CShaderBase* alternate[4];
static unsigned ids[16];
static bool dead[8];
static void check(bool v){if(!v)++failed;}
void ResourceBoundary::BaseConstruct(){check(calls==0 && this==reinterpret_cast<ResourceBoundary*>(owner));++calls;}
void ResourceBoundary::InputConstruct(){check(calls==1 && this==reinterpret_cast<ResourceBoundary*>(reinterpret_cast<char*>(owner)+24));++calls;memset(this,0x73,28);}
class ProbeShader : public CShaderBase {
 unsigned id;
public:
 explicit ProbeShader(unsigned value):id(value){}
 virtual ~ProbeShader(){
  check(id<8 && !dead[id]);dead[id]=true;ids[events++]=id;
  if(events==1 && mode==1) CShaderResourceTestAccess::Set(owner,alternate,4);
  if(events==1 && mode==2) CShaderResourceTestAccess::Set(owner,CShaderResourceTestAccess::Begin(owner),0);
  if(events==1 && mode==3) CShaderResourceTestAccess::Set(owner,CShaderResourceTestAccess::Begin(owner),4);
  CShaderResourceTestAccess::SetBank(owner,reinterpret_cast<CShaderDataBank*>(0x9876));
  CShaderResourceTestAccess::SetManager(owner,reinterpret_cast<CShaderRenderManager*>(0x1357));
 }
private:virtual unsigned long CountReferences(){return 1;}
};
int main(){
 unsigned cases=0;
 for(unsigned fill=0;fill<256;++fill){
  unsigned long raw[15];memset(raw,fill,sizeof(raw));owner=reinterpret_cast<CShaderResource*>(raw);calls=0;
  check(CShaderResourceTestAccess::Construct(raw)==owner && calls==2);
  for(unsigned i=1;i<=5;++i)check(raw[i]==0);
  check(raw[13]==0 && raw[14]==0);
  for(i=6;i<13;++i)check(raw[i]==0x73737373);
  ++cases;
 }
 for(unsigned mask=0;mask<16;++mask)for(mode=0;mode<4;++mode)for(unsigned uninit=0;uninit<2;++uninit)for(unsigned count=0;count<5;++count){
  unsigned long raw[15];memset(raw,0xa5,sizeof(raw));owner=reinterpret_cast<CShaderResource*>(raw);
  CShaderBase* initial[4];ProbeShader* allocated[8];
  memset(dead,0,sizeof(dead));events=0;
  for(unsigned i=0;i<8;++i)allocated[i]=new ProbeShader(i);
  for(i=0;i<4;++i){initial[i]=(mask&(1<<i))?allocated[i]:0;alternate[i]=allocated[4+i];}
  CShaderResourceTestAccess::Set(owner,initial,count);
  CShaderResourceTestAccess::SetBank(owner,reinterpret_cast<CShaderDataBank*>(0x1111));
  CShaderResourceTestAccess::SetManager(owner,reinterpret_cast<CShaderRenderManager*>(0x2222));
  unsigned expect[8],n=0,limit=count;bool swapped=false;
  for(i=0;i<limit;++i){
   if(swapped || (mask&(1<<i))){
    expect[n++]=swapped?i+4:i;
    if(n==1){if(mode==1){swapped=true;limit=4;}else if(mode==2)limit=0;else if(mode==3)limit=4;}
   }
  }
  if(uninit)CShaderResourceTestAccess::Uninit(owner);else owner->CloseShaderBank();
  check(events==n);
  for(i=0;i<n;++i)check(ids[i]==expect[i]);
  check(!CShaderResourceTestAccess::Bank(owner));
  check(CShaderResourceTestAccess::Manager(owner)==reinterpret_cast<CShaderRenderManager*>(uninit?0:n?0x1357:0x2222));
  for(i=0;i<n;++i){unsigned index=expect[i]%4;check((swapped?alternate:initial)[index]==0);}
  unsigned savedMode=mode;mode=0;
  for(i=0;i<8;++i)if(!dead[i])delete allocated[i];
  mode=savedMode;++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("SHADER_RESOURCE_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
#pragma comment(linker,"/alternatename:??1CBaseClass@@UAE@XZ=_ResourceUnexpectedCall")

#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=_ResourceUnexpectedCall")
