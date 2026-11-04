
#include <stdio.h>
#include <string.h>
#include "fable_shader_resource.h"
struct CShaderResourceTestAccess {
 static CVertexShaderInput& Input(CShaderResource& r){return r.VertexShaderInput;}
 static D3DVERTEXELEMENT9* Declaration(CShaderResource& r){return r.VertexShaderDeclaration;}
};
typedef _STL::vector<CVertexShaderInput::CStreamInfo> Streams;
typedef _STL::vector<D3DVERTEXELEMENT9> Declarations;
static CShaderResource* owner;
static CVertexShaderInput* source;
static unsigned failed,calls,streamMode,declarationMode,count;
static unsigned long streamWords[30],declarationWords[20],alternateWords[20];
static void check(bool v){if(!v)++failed;}
namespace _STL {
template<> Streams& Streams::operator=(const Streams& other){
 check(calls++==0 && this==&CShaderResourceTestAccess::Input(*owner).DataStreams && &other==&source->DataStreams);
 if(streamMode==1){source->D3DDeclaration._M_start=reinterpret_cast<D3DVERTEXELEMENT9*>(alternateWords);source->D3DDeclaration._M_finish=source->D3DDeclaration._M_start+count;}
 if(streamMode==2){CShaderResourceTestAccess::Input(*owner).D3DDeclaration._M_start=reinterpret_cast<D3DVERTEXELEMENT9*>(declarationWords);CShaderResourceTestAccess::Input(*owner).D3DDeclaration._M_finish=reinterpret_cast<D3DVERTEXELEMENT9*>(declarationWords)+count;}
 this->_M_start=reinterpret_cast<CVertexShaderInput::CStreamInfo*>(streamWords);this->_M_finish=this->_M_start+1;this->_M_end_of_storage._M_data=this->_M_start+2;
 return *this;
}
template<> Declarations& Declarations::operator=(const Declarations& other){
 check(calls++==1 && this==&CShaderResourceTestAccess::Input(*owner).D3DDeclaration && &other==&source->D3DDeclaration);
 D3DVERTEXELEMENT9* first=other._M_start;
 if(declarationMode==1)first=reinterpret_cast<D3DVERTEXELEMENT9*>(alternateWords);
 if(declarationMode==2)first=0;
 this->_M_start=first;this->_M_finish=first+(declarationMode>=2?0:count);this->_M_end_of_storage._M_data=this->_M_finish;
 return *this;
}
}
int main(){unsigned cases=0;const unsigned fills[]={0,1,0x55,0xa5};
 for(unsigned f=0;f<4;++f)for(unsigned self=0;self<2;++self)for(streamMode=0;streamMode<3;++streamMode)for(declarationMode=0;declarationMode<4;++declarationMode)for(count=0;count<3;++count){
  unsigned long storage[15],inputWords[7];memset(storage,fills[f],sizeof(storage));memset(inputWords,0x66,sizeof(inputWords));owner=reinterpret_cast<CShaderResource*>(storage);source=self?&CShaderResourceTestAccess::Input(*owner):reinterpret_cast<CVertexShaderInput*>(inputWords);
  source->D3DDeclaration._M_start=reinterpret_cast<D3DVERTEXELEMENT9*>(declarationWords);source->D3DDeclaration._M_finish=source->D3DDeclaration._M_start+count;source->D3DDeclaration._M_end_of_storage._M_data=source->D3DDeclaration._M_finish;
  unsigned long preserved[15];memcpy(preserved,storage,sizeof(storage));calls=0;owner->SetVertexShaderInput(*source);check(calls==2);
  CVertexShaderInput& output=CShaderResourceTestAccess::Input(*owner);D3DVERTEXELEMENT9* expected=count && declarationMode<2?output.D3DDeclaration._M_start:reinterpret_cast<D3DVERTEXELEMENT9*>(&output.D3DDeclaration);
  check(CShaderResourceTestAccess::Declaration(*owner)==expected);
  for(unsigned i=0;i<7;++i)check(storage[i]==preserved[i]);check(storage[14]==preserved[14]);
  ++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("SHADER_RESOURCE_INPUT_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
