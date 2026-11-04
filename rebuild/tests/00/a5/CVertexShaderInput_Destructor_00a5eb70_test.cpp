#include "fable_vertex_shader_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class InputDestructorEntry {public:void Destroy();void* Dispose(unsigned);};
#pragma comment(linker,"/alternatename:?Destroy@InputDestructorEntry@@QAEXXZ=??1CVertexShaderInput@@UAE@XZ")
#pragma comment(linker,"/alternatename:?Dispose@InputDestructorEntry@@QAEPAXI@Z=??_GCVertexShaderInput@@UAEPAXI@Z")
extern "C" void __fastcall DestroyStreamRange(CVertexShaderInput::CStreamInfo*,CVertexShaderInput::CStreamInfo*,const _STL::__false_type&);
#pragma comment(linker,"/alternatename:@DestroyStreamRange@12=??$__destroy_aux@PAVCStreamInfo@CVertexShaderInput@@@_STL@@YIXPAVCStreamInfo@CVertexShaderInput@@0ABU__false_type@0@@Z")
static unsigned count,mode,events,failed,deletes,kind;
static CVertexShaderInput* owner;
static CVertexShaderInput::CStreamInfo* original;
static CVertexShaderInput::CStreamInfo* replacement;
static void check(bool v){if(!v)++failed;}
CBaseClass::CBaseClass(){abort();}
void operator delete(void* p){check(p==owner);++deletes;free(p);}
CBaseClass::~CBaseClass(){
 if(this==owner){check(events==count);++events;}
 else {
  check(events<count && this==&original[events].StreamFormat);++events;
  if(events==1 && mode && kind!=4){owner->DataStreams._M_start=mode==1?replacement:0;owner->DataStreams._M_finish=replacement;}
 }
}
int main(){
 unsigned cases=0;
 for(count=0;count<5;++count)for(unsigned mask=0;mask<16;++mask)for(unsigned declaration=0;declaration<2;++declaration)for(mode=0;mode<3;++mode)for(kind=0;kind<5;++kind){
  owner=static_cast<CVertexShaderInput*>(malloc(sizeof(*owner)));memset(owner,0x55,sizeof(*owner));
  original=static_cast<CVertexShaderInput::CStreamInfo*>(malloc(80));memset(original,0x6a,80);
  replacement=static_cast<CVertexShaderInput::CStreamInfo*>(malloc(20));
  for(unsigned i=0;i<count;++i){original[i].StreamFormat.DataElements._M_start=(mask&(1<<i))?static_cast<CVertexStreamFormat::CDataElement*>(malloc(8)):0;}
  owner->DataStreams._M_start=original;owner->DataStreams._M_finish=original+count;owner->DataStreams._M_end_of_storage._M_data=original+4;
  D3DVERTEXELEMENT9* decl=declaration?static_cast<D3DVERTEXELEMENT9*>(malloc(16)):0;
  owner->D3DDeclaration._M_start=decl;
  events=deletes=0;
  if(kind==4){_STL::__false_type tag;DestroyStreamRange(original,original+count,tag);check(events==count && deletes==0);free(decl);}
  else if(kind==0){reinterpret_cast<InputDestructorEntry*>(owner)->Destroy();}
  else {unsigned flags=kind==1?0:kind==2?1:3;void* result=reinterpret_cast<InputDestructorEntry*>(owner)->Dispose(flags);check(result==owner);}
  if(kind!=4)check(events==count+1 && deletes==(kind>=2?1:0));
  if(kind==4 || (count && mode))free(original);
  if(kind==4 || !count || mode!=1)free(replacement);
  if(kind==0 || kind==1 || kind==4)free(owner);
  ++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("VERTEX_INPUT_DESTRUCTOR_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
