#include <stdio.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
struct CVertexShaderInputTestAccess { static void generate(CVertexShaderInput& input){input.GenerateD3DDeclaration();} };
static unsigned failed,calls,expectedCount;
static unsigned char alternate[4096];
static _STL::vector<D3DVERTEXELEMENT9>* owner;
static D3DVERTEXELEMENT9* expectedPosition;
static void check(bool value){if(!value)++failed;}
namespace _STL {
template<> void vector<D3DVERTEXELEMENT9>::_M_fill_insert(D3DVERTEXELEMENT9* position,size_t count,const D3DVERTEXELEMENT9& value){
 ++calls;check(this==owner && position==expectedPosition && count==expectedCount);
 const unsigned char* bytes=reinterpret_cast<const unsigned char*>(&value);
 for(unsigned j=0;j<8;++j)check(bytes[j]==0);
 unsigned used=size(),total=used+count;
 if(total>capacity()){
  if(used)memcpy(alternate,_M_start,used*8);
  _M_start=reinterpret_cast<D3DVERTEXELEMENT9*>(alternate);_M_end_of_storage._M_data=_M_start+total;
 }
 for(unsigned i=0;i<count;++i)_M_start[used+i]=value;
 _M_finish=_M_start+total;
}
}
struct Pattern {unsigned streams;unsigned counts[3];unsigned types[270];};
struct Scenario {unsigned pattern,used,capacity,hash;};
#include "test_support/vertex_declaration_generation_expected.h"
static unsigned hashBytes(unsigned hash,const void* source,unsigned length){const unsigned char* p=static_cast<const unsigned char*>(source);for(unsigned i=0;i<length;++i)hash=(hash^p[i])*16777619U;return hash;}
int main(){unsigned count=0;
 for(unsigned k=0;k<sizeof(scenarios)/sizeof(scenarios[0]);++k){
  const Scenario& scenario=scenarios[k];const Pattern& pattern=patterns[scenario.pattern];
  unsigned long object[7]={0},streamWords[15]={0};unsigned long elements[3][540]={0};unsigned char declaration[4096];
  memset(declaration,0xcd,sizeof(declaration));memset(alternate,0xab,sizeof(alternate));
  CVertexShaderInput& input=*reinterpret_cast<CVertexShaderInput*>(object);
  input.DataStreams._M_start=reinterpret_cast<CVertexShaderInput::CStreamInfo*>(streamWords);
  input.DataStreams._M_finish=input.DataStreams._M_start+pattern.streams;input.DataStreams._M_end_of_storage._M_data=input.DataStreams._M_finish;
  unsigned cursor=0,total=0;
  for(unsigned i=0;i<pattern.streams;++i){
   streamWords[5*i]=57+i;streamWords[5*i+2]=reinterpret_cast<unsigned long>(elements[i]);streamWords[5*i+3]=streamWords[5*i+2]+8*pattern.counts[i];streamWords[5*i+4]=streamWords[5*i+3];
   for(unsigned j=0;j<pattern.counts[i];++j){elements[i][2*j]=0x123400fe+j;elements[i][2*j+1]=pattern.types[cursor++];}
   total+=pattern.counts[i]+1;
  }
  D3DVERTEXELEMENT9* begin=scenario.capacity?reinterpret_cast<D3DVERTEXELEMENT9*>(declaration):0;
  input.D3DDeclaration._M_start=begin;input.D3DDeclaration._M_finish=begin+scenario.used;input.D3DDeclaration._M_end_of_storage._M_data=begin+scenario.capacity;
  owner=&input.D3DDeclaration;calls=0;expectedCount=total-scenario.used;expectedPosition=begin+scenario.used;
  CVertexShaderInputTestAccess::generate(input);
  unsigned metadata[4]={input.D3DDeclaration.size(),input.D3DDeclaration.capacity(),input.D3DDeclaration._M_start==reinterpret_cast<D3DVERTEXELEMENT9*>(alternate),calls};
  unsigned hash=hashBytes(2166136261U,metadata,sizeof(metadata));hash=hashBytes(hash,declaration,4096);hash=hashBytes(hash,alternate,4096);check(hash==scenario.hash);++count;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("DECLARATION_GENERATION_BEHAVIOR_PASS scenarios=%u\n",count);return 0;
}
