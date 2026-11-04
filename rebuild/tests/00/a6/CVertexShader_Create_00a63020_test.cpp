
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader.h"
#include "fable_d3d9_shader_create.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
struct CSystemManagerTestAccess {static void SetDisplay(CSystemManager& s,CDisplayManager* d){s.PDisplayManager.Data=d;}};
struct CVertexShaderTestAccess {
 static CVertexShader* Create(void* storage,const CArrayVector<unsigned long>& code,const D3DVERTEXELEMENT9* elements,const CCharString& name){return new(storage) CVertexShader(code,elements,name,name);}
 static CVertexShaderData* Data(CVertexShader* p){return p->Data;}
 static void Destroy(CVertexShader* p){p->CVertexShader::~CVertexShader();}
 static bool Contents(CVertexShaderData* p,IDirect3DVertexDeclaration9* declaration,IDirect3DVertexShader9* shader,const D3DVERTEXELEMENT9* elements){return p->VertexDeclaration==declaration && p->VertexShader==shader && p->FormatDecl==elements && p->ReferenceCount==1;}
};
static unsigned errors,events[16],eventCount,mask,failAllocation,mutation,failureResult;
static bool recordAllocation;
static CVertexShader* owner;
static void* allocation;
static CSystemManager* systemManager;
static CDisplayManager* display;
static const DWORD* expectedCode;
static const D3DVERTEXELEMENT9* expectedElements;
static FableD3D9ShaderCreationVTable tables[2];
static void* devices[2][1];static void* shaderObject[1];static void* declarationObject[1];static void* interfaceTable[3];
static void check(bool v){if(!v)++errors;}
static void event(unsigned v){events[eventCount++]=v;}
void* operator new(size_t n){if(recordAllocation){event(4);check(n==20);if(failAllocation)return 0;}void* p=malloc(n);if(recordAllocation){allocation=p;memset(p,0x66,n);}return p;}
void operator delete(void* p){if(p==allocation)event(9);free(p);}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){check(this==allocation);event(5);}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){event(8);}
CCharString::CCharString():storage_(0){} CCharString::~CCharString(){}
CCharString CShaderBase::GetDebugName() const{return CCharString();}
CSystemManager* __fastcall GFGetSystemManager(){event(1);return systemManager;}
static unsigned long __stdcall Release(void* p){check(p==shaderObject || p==declarationObject);event(p==shaderObject?6:7);return 0;}
static HRESULT __stdcall CreateShader(IDirect3DDevice9* d,const DWORD* code,IDirect3DVertexShader9** result){
 check(d==reinterpret_cast<IDirect3DDevice9*>(devices[0]) && code==expectedCode && eventCount==1);event(2);*result=mask&1?reinterpret_cast<IDirect3DVertexShader9*>(shaderObject):0;
 if(mutation==1)display->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(devices[1]);
 if(mutation==2)devices[0][0]=&tables[1];
 return failureResult?static_cast<HRESULT>(0x80004005):0;
}
static HRESULT __stdcall CreateDeclaration(IDirect3DDevice9* d,const D3DVERTEXELEMENT9* elements,IDirect3DVertexDeclaration9** result){
 check(d==reinterpret_cast<IDirect3DDevice9*>(devices[0]) && elements==expectedElements && eventCount==2 && mutation!=2);event(3);*result=mask&2?reinterpret_cast<IDirect3DVertexDeclaration9*>(declarationObject):0;return failureResult?static_cast<HRESULT>(0x80004005):0;
}
static HRESULT __stdcall AlternateDeclaration(IDirect3DDevice9* d,const D3DVERTEXELEMENT9* elements,IDirect3DVertexDeclaration9** result){
 check(d==reinterpret_cast<IDirect3DDevice9*>(devices[0]) && elements==expectedElements && eventCount==2 && mutation==2);event(3);*result=mask&2?reinterpret_cast<IDirect3DVertexDeclaration9*>(declarationObject):0;return failureResult?static_cast<HRESULT>(0x80004005):0;
}
int main(){unsigned cases=0;tables[0].CreateVertexShader=CreateShader;tables[0].CreateVertexDeclaration=CreateDeclaration;tables[1].CreateVertexDeclaration=AlternateDeclaration;interfaceTable[2]=reinterpret_cast<void*>(&Release);shaderObject[0]=declarationObject[0]=interfaceTable;
 unsigned long systemWords[sizeof(CSystemManager)/4],displayWords[sizeof(CDisplayManager)/4];systemManager=reinterpret_cast<CSystemManager*>(systemWords);display=reinterpret_cast<CDisplayManager*>(displayWords);memset(systemWords,0,sizeof(systemWords));memset(displayWords,0,sizeof(displayWords));CSystemManagerTestAccess::SetDisplay(*systemManager,display);CCharString name;
 const unsigned counts[]={0,1,5},fills[]={0,0x55,0xa5};
 for(unsigned c=0;c<3;++c)for(unsigned spare=0;spare<2;++spare)for(unsigned format=0;format<2;++format)for(mask=0;mask<4;++mask)for(failureResult=0;failureResult<2;++failureResult)for(failAllocation=0;failAllocation<2;++failAllocation)for(mutation=0;mutation<3;++mutation)for(unsigned f=0;f<3;++f){
  unsigned long ownerWords[2],arrayWords[3],code[8];memset(ownerWords,fills[f],sizeof(ownerWords));CArrayVector<unsigned long>& array=*reinterpret_cast<CArrayVector<unsigned long>*>(arrayWords);unsigned cap=counts[c]+spare;array._M_start=cap?code:0;array._M_finish=array._M_start+counts[c];array._M_end_of_storage._M_data=array._M_start+cap;expectedCode=counts[c]?code:arrayWords;expectedElements=format?reinterpret_cast<const D3DVERTEXELEMENT9*>(0x12345678):0;
  devices[0][0]=&tables[0];devices[1][0]=0;display->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(devices[0]);allocation=0;owner=reinterpret_cast<CVertexShader*>(ownerWords);eventCount=0;recordAllocation=true;
  check(CVertexShaderTestAccess::Create(ownerWords,array,expectedElements,name)==owner);recordAllocation=false;
  check(eventCount==(failAllocation?4:5));for(unsigned i=0;i<eventCount;++i)check(events[i]==i+1);
  CVertexShaderData* data=CVertexShaderTestAccess::Data(owner);check(data==allocation);
  if(data)check(CVertexShaderTestAccess::Contents(data,mask&2?reinterpret_cast<IDirect3DVertexDeclaration9*>(declarationObject):0,mask&1?reinterpret_cast<IDirect3DVertexShader9*>(shaderObject):0,expectedElements));
  CVertexShaderTestAccess::Destroy(owner);unsigned expected=failAllocation?4:5;if(!failAllocation){if(mask&1)check(events[expected++]==6);if(mask&2)check(events[expected++]==7);check(events[expected++]==8);check(events[expected++]==9);}check(eventCount==expected);++cases;
 }
 if(errors){printf("FAIL %u\n",errors);return 1;}printf("VERTEX_SHADER_CREATE_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
