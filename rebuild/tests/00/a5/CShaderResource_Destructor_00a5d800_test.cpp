#include "fable_shader_resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct CShaderResourceTestAccess {
 static _STL::vector<CShaderBase*>& Shaders(CShaderResource* p){return p->Shaders;}
 static CVertexShaderInput& Input(CShaderResource* p){return p->VertexShaderInput;}
 static bool Closed(CShaderResource* p){return !p->ShaderBank && !p->ShaderRenderManager;}
};
class ResourceDestructorEntry {public:void* Dispose(unsigned);void BaseDestroy();};
#pragma comment(linker,"/alternatename:?Dispose@ResourceDestructorEntry@@QAEPAXI@Z=??_GCShaderResource@@UAEPAXI@Z")
#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=?BaseDestroy@ResourceDestructorEntry@@QAEXXZ")
extern "C" void ResourceDestructorUnexpected(){abort();}
#pragma comment(linker,"/alternatename:??0CBaseClassNonCopyable@@QAE@XZ=_ResourceDestructorUnexpected")
#pragma comment(linker,"/alternatename:??0CVertexShaderInput@@QAE@XZ=_ResourceDestructorUnexpected")
#pragma comment(linker,"/alternatename:?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ=_ResourceDestructorUnexpected")
static unsigned failed,count,mode,streamCalls,inputCalls,baseCalls,ownerDeletes;
static CShaderResource* owner;
static CVertexShaderInput::CStreamInfo* streams;
static CShaderBase** original;
static CShaderBase** replacement;
static bool dead[4],mutated,cleanup;
static void check(bool v){if(!v)++failed;}
void operator delete(void* p){if(p==owner)++ownerDeletes;free(p);}
CBaseClass::CBaseClass(){abort();}
CBaseClass::~CBaseClass(){
 check(CShaderResourceTestAccess::Closed(owner));
 if(this==&CShaderResourceTestAccess::Input(owner)){
  check(streamCalls==count);++inputCalls;
  if(mode==3){CShaderResourceTestAccess::Shaders(owner)._M_start=0;mutated=true;}
 }else{
  check(streamCalls<count && this==&streams[streamCalls].StreamFormat);++streamCalls;
  if(mode==2 && streamCalls==1){CShaderResourceTestAccess::Shaders(owner)._M_start=replacement;mutated=true;}
 }
}
void ResourceDestructorEntry::BaseDestroy(){check(this==reinterpret_cast<ResourceDestructorEntry*>(owner) && inputCalls==1 && streamCalls==count);++baseCalls;}
class ShaderProbe : public CShaderBase {
 unsigned id;
public:
 explicit ShaderProbe(unsigned value):id(value){}
 virtual ~ShaderProbe(){check(!dead[id]);dead[id]=true;if(!cleanup && mode==1 && !mutated){CShaderResourceTestAccess::Shaders(owner)._M_start=replacement;CShaderResourceTestAccess::Shaders(owner)._M_finish=replacement;mutated=true;}}
private:virtual unsigned long CountReferences(){return 1;}
};
int main(){unsigned cases=0;
 for(count=0;count<4;++count)for(unsigned mask=0;mask<16;++mask)for(unsigned elements=0;elements<2;++elements)for(unsigned declaration=0;declaration<2;++declaration)for(mode=0;mode<4;++mode)for(unsigned kind=0;kind<4;++kind){
  owner=static_cast<CShaderResource*>(malloc(sizeof(*owner)));memset(owner,0x55,sizeof(*owner));
  original=static_cast<CShaderBase**>(malloc(16));replacement=static_cast<CShaderBase**>(malloc(16));memset(replacement,0,16);
  ShaderProbe* allocated[4];memset(dead,0,sizeof(dead));cleanup=mutated=false;
  for(unsigned i=0;i<4;++i){allocated[i]=new ShaderProbe(i);original[i]=(mask&(1<<i))?allocated[i]:0;}
  _STL::vector<CShaderBase*>& shaders=CShaderResourceTestAccess::Shaders(owner);shaders._M_start=original;shaders._M_finish=original+4;shaders._M_end_of_storage._M_data=original+4;
  streams=static_cast<CVertexShaderInput::CStreamInfo*>(malloc(80));memset(streams,0x6a,80);
  for(i=0;i<count;++i)streams[i].StreamFormat.DataElements._M_start=elements?static_cast<CVertexStreamFormat::CDataElement*>(malloc(8)):0;
  CVertexShaderInput& input=CShaderResourceTestAccess::Input(owner);input.DataStreams._M_start=streams;input.DataStreams._M_finish=streams+count;input.DataStreams._M_end_of_storage._M_data=streams+4;input.D3DDeclaration._M_start=declaration?static_cast<D3DVERTEXELEMENT9*>(malloc(16)):0;
  streamCalls=inputCalls=baseCalls=ownerDeletes=0;
  if(kind==0)owner->CShaderResource::~CShaderResource();
  else {void* result=reinterpret_cast<ResourceDestructorEntry*>(owner)->Dispose(kind==1?0:kind==2?1:3);check(result==owner);}
  check(baseCalls==1 && inputCalls==1 && streamCalls==count && ownerDeletes==(kind>=2?1:0));
  if(mode!=1 || !mutated)for(i=0;i<4;++i)check(dead[i]==bool(mask&(1<<i)));
  if(mutated)free(original);
  if(!(mutated && (mode==1 || mode==2)))free(replacement);
  cleanup=true;for(i=0;i<4;++i)if(!dead[i])delete allocated[i];
  if(kind<2)free(owner);
  ++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("SHADER_RESOURCE_DESTRUCTOR_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;}
