#include "fable_shader_resource.h"
#include "fable_shader_render_manager.h"
#include "fable_counted_pointer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct CShaderResourceTestAccess {
 static void Init(CShaderResource* p,CShaderRenderManager& m){p->Initialise(m);}
 static void Bank(CShaderResource* p,CShaderDataBank* bank){p->ShaderBank=bank;}
};
static unsigned failures,callbackMode,callbackCalls,deleteCalls;
static bool cleanup,freed[3];
static CCPPointerInfo* blocks[3];
static CCountedPointer<CShaderResource>* destination;
static CCountedPointer<CShaderResource>* source;
static void check(bool v){if(!v)++failures;}
void operator delete(void* p){
 if(!cleanup){++deleteCalls;for(unsigned i=0;i<3;++i)if(p==blocks[i])freed[i]=true;}
 free(p);
}
static void __fastcall ReleaseCallback(void* p){
 check(p==reinterpret_cast<void*>(0x3333));++callbackCalls;
 if(callbackMode==1)destination->Info=blocks[2];
 if(callbackMode==2){source->Data=reinterpret_cast<CShaderResource*>(0x7777);source->Info=blocks[2];}
 if(callbackMode==3)destination->Info=0;
}
int main(){unsigned cases=0;
 CShaderRenderManager managers[2];D3DVERTEXELEMENT9 declarations[2];
 for(unsigned fill=0;fill<256;++fill)for(unsigned choice=0;choice<4;++choice){
  unsigned long raw[15],expected[15];memset(raw,fill,sizeof(raw));memcpy(expected,raw,sizeof(raw));CShaderResource* r=reinterpret_cast<CShaderResource*>(raw);
  CShaderResourceTestAccess::Init(r,managers[choice%2]);expected[5]=reinterpret_cast<unsigned long>(&managers[choice%2]);check(!memcmp(raw,expected,sizeof(raw)));
  const D3DVERTEXELEMENT9* decl=choice?&declarations[choice%2]:0;r->SetVertexShaderDeclaration(decl);expected[13]=reinterpret_cast<unsigned long>(decl);check(!memcmp(raw,expected,sizeof(raw)));
  const unsigned long formats[4]={0,0x144,0x80000000UL,0xffffffffUL};r->SetVertexFormat(static_cast<EVertexType>(formats[choice]));expected[14]=formats[choice];check(!memcmp(raw,expected,sizeof(raw)));
  CShaderDataBank* bank=reinterpret_cast<CShaderDataBank*>(choice*0x1111);CShaderResourceTestAccess::Bank(r,bank);expected[1]=reinterpret_cast<unsigned long>(bank);check(r->PeekShaderBank()==bank && !memcmp(raw,expected,sizeof(raw)));++cases;
 }
 const unsigned long counts[5]={0,0,1,2,0xffffffffUL};
 for(unsigned oldCase=0;oldCase<5;++oldCase)for(unsigned newCase=0;newCase<5;++newCase)for(unsigned alias=0;alias<3;++alias)for(callbackMode=0;callbackMode<4;++callbackMode){
  cleanup=false;memset(freed,0,sizeof(freed));callbackCalls=deleteCalls=0;
  for(unsigned i=0;i<3;++i)blocks[i]=new CCPPointerInfo(ReleaseCallback,reinterpret_cast<void*>(0x3333));
  blocks[0]->RefCount=counts[oldCase];blocks[1]->RefCount=counts[newCase];
  unsigned long lhs[2],rhs[2];destination=reinterpret_cast<CCountedPointer<CShaderResource>*>(lhs);source=reinterpret_cast<CCountedPointer<CShaderResource>*>(rhs);
  destination->Data=reinterpret_cast<CShaderResource*>(0xaaaa);destination->Info=oldCase?blocks[0]:0;
  source->Data=reinterpret_cast<CShaderResource*>(0xbbbb);source->Info=alias==1?destination->Info:newCase?blocks[1]:0;
  if(alias==2)source=destination;
  CCPPointerInfo* old=destination->Info;CCPPointerInfo* incoming=source->Info;CShaderResource* incomingData=source->Data;
  bool changed=old!=incoming;bool released=changed && old && counts[oldCase]==1;
  unsigned long incomingCount=incoming?incoming->RefCount:0;
  CCountedPointer<CShaderResource>& result=(*destination=*source);
  check(&result==destination);
  check(destination->Info==incoming && destination->Data==(changed?incomingData:reinterpret_cast<CShaderResource*>(0xaaaa)));
  check(callbackCalls==(released?1:0) && deleteCalls==(released?1:0));
  if(incoming)check(incoming->RefCount==incomingCount+(changed?1:0));
  if(old && !freed[0])check(old->RefCount==counts[oldCase]-(changed?1:0));
  if(released)check(callbackMode==1?freed[2]:callbackMode==3?!freed[0]:freed[0]);
  cleanup=true;for(i=0;i<3;++i)if(!freed[i])delete blocks[i];
  ++cases;
 }
 if(failures){printf("FAIL %u\n",failures);return 1;}
 printf("SHADER_RESOURCE_BINDINGS_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;}
