#include <new>
#include <stdio.h>
#include <string.h>
#include "fable_vertex_shader.h"
struct CVertexShaderTestAccess {
 static CVertexShaderData* Create(void* memory,IDirect3DVertexDeclaration9* declaration,IDirect3DVertexShader9* shader,const D3DVERTEXELEMENT9* format,const CCharString& name){return new(memory) CVertexShaderData(declaration,shader,format,name,name);}
 static void Destroy(CVertexShaderData* p){p->CVertexShaderData::~CVertexShaderData();}
 static void SetCount(CVertexShaderData* p,unsigned long n){p->ReferenceCount=n;}
 static unsigned long Count(CVertexShaderData* p){return p->ReferenceCount;}
 static unsigned long Count(CVertexShader* p){return p->CountReferences();}
 static void SetData(CVertexShader* p,CVertexShaderData* data){p->Data=data;}
 static CVertexShaderData* Data(CVertexShader* p){return p->Data;}
 static void SetDeclaration(CVertexShaderData* p,IDirect3DVertexDeclaration9* d){p->VertexDeclaration=d;}
 static void Delete(CVertexShaderData* p){delete p;}
};
typedef CVertexShaderTestAccess A;
static CVertexShaderData* data;static int errors,mutation,events[12],eventCount,baseCtors,baseDtors;
static void* shader[1];static void* declaration[2][1];static void* shaderVtable[3];static void* declVtable[3];
static void check(bool b){if(!b)++errors;}static void event(int e){events[eventCount++]=e;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){check(this==data);++baseCtors;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){check(this==data);++baseDtors;event(3);}
CCharString::CCharString():storage_(0){} CCharString::~CCharString(){}
CCharString CShaderBase::GetDebugName() const {return CCharString();}
static unsigned long FABLE_STDCALL ReleaseShader(void* p){
 check(p==shader);event(1);if(mutation==1)A::SetDeclaration(data,reinterpret_cast<IDirect3DVertexDeclaration9*>(declaration[1]));else if(mutation==2)A::SetDeclaration(data,0);return 123;
}
static unsigned long FABLE_STDCALL ReleaseDeclaration(void* p){check(p==declaration[(mutation==1 && eventCount>0 && events[0]==1)?1:0]);event(2);return 456;}
__declspec(noinline) static void DeleteShader(CVertexShader* p){delete p;}
int main(){unsigned scenarios=0;shader[0]=shaderVtable;declaration[0][0]=declaration[1][0]=declVtable;shaderVtable[2]=reinterpret_cast<void*>(&ReleaseShader);declVtable[2]=reinterpret_cast<void*>(&ReleaseDeclaration);CCharString name;
 for(unsigned fill=0;fill<256;++fill)for(int mask=0;mask<4;++mask)for(mutation=0;mutation<3;++mutation){
  unsigned long storage[5],expected[5];memset(storage,fill,sizeof(storage));data=reinterpret_cast<CVertexShaderData*>(storage);baseCtors=baseDtors=eventCount=0;
  A::Create(storage,(mask&1)?reinterpret_cast<IDirect3DVertexDeclaration9*>(declaration[0]):0,(mask&2)?reinterpret_cast<IDirect3DVertexShader9*>(shader):0,reinterpret_cast<const D3DVERTEXELEMENT9*>(0x456700),name);
  check(baseCtors==1 && storage[1]==((mask&1)?reinterpret_cast<unsigned long>(declaration[0]):0) && storage[2]==((mask&2)?reinterpret_cast<unsigned long>(shader):0) && storage[3]==0x456700 && storage[4]==1);
  memcpy(expected,storage,sizeof(storage));A::Destroy(data);check(baseDtors==1);
  bool releaseDecl=(mask&2)?(mutation==1 || (mutation==0 && (mask&1))):(mask&1)!=0;
  int n=0;if(mask&2)check(events[n++]==1);if(releaseDecl)check(events[n++]==2);check(events[n++]==3 && eventCount==n);
  if((mask&2)&&mutation)expected[1]=mutation==1?reinterpret_cast<unsigned long>(declaration[1]):0;
  check(memcmp(storage+1,expected+1,16)==0);++scenarios;
 }
 const unsigned long counts[]={0,1,2,17,0xffffffff};
 for(unsigned fill=0;fill<256;++fill)for(int c=0;c<5;++c)for(int attached=0;attached<2;++attached){
  mutation=0;void* memory=::operator new(20);memset(memory,fill,20);data=static_cast<CVertexShaderData*>(memory);A::Create(memory,0,0,0,name);A::SetCount(data,counts[c]);
  CVertexShader* handle=new CVertexShader;check(A::Data(handle)==0);A::SetData(handle,attached?data:0);if(attached)check(A::Count(handle)==counts[c]);baseDtors=eventCount=0;
  DeleteShader(handle);check(baseDtors==((attached&&counts[c]==1)?1:0));
  if(!attached||counts[c]!=1){check(A::Count(data)==counts[c]-(attached?1:0));A::Delete(data);}++scenarios;
 }
 printf("VERTEX_SHADER_LIFETIME_BEHAVIOR_%s scenarios=%u failures=%d\n",errors?"FAIL":"PASS",scenarios,errors);return errors?1:0;
}


#include <stdlib.h>
extern "C" void NeverCalledShaderSystemManager(){abort();}
#pragma comment(linker,"/alternatename:?GFGetSystemManager@@YIPAVCSystemManager@@XZ=_NeverCalledShaderSystemManager")
