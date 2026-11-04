#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fable_vertex_shader_input.h"
struct CVertexShaderInputTestAccess { static void generate(CVertexShaderInput& input){input.GenerateD3DDeclaration();} };
static unsigned failed;
static void check(bool value){if(!value)++failed;}
struct Pattern {unsigned streams;unsigned counts[3];unsigned types[270];};
struct Scenario {unsigned pattern,used,capacity,hash;};
#include "test_support/vertex_declaration_generation_expected.h"
int main(){unsigned count=0;
 for(unsigned k=0;k<sizeof(scenarios)/sizeof(scenarios[0]);++k){
  const Scenario& scenario=scenarios[k];const Pattern& pattern=patterns[scenario.pattern];
  unsigned long object[7]={0},streamWords[15]={0};unsigned long elements[3][540]={0};unsigned char* declaration=scenario.capacity?static_cast<unsigned char*>(malloc(scenario.capacity*8)):0;
  if(declaration)memset(declaration,0xcd,scenario.capacity*8);
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
  CVertexShaderInputTestAccess::generate(input);
  bool supported=true;unsigned output=0,offset=0,cursor2=0;
  const unsigned sizes[8]={4,8,12,16,4,4,8,4},types[8]={0,1,2,3,4,6,7,6};
  for(i=0;i<pattern.streams && supported;++i)for(unsigned j=0;j<pattern.counts[i];++j){
   unsigned type=pattern.types[cursor2++];if(type>=8){supported=false;break;}
   D3DVERTEXELEMENT9 expected={static_cast<unsigned short>(i),static_cast<unsigned short>(offset),static_cast<unsigned char>(types[type]),0,5,static_cast<unsigned char>(0xfe+j)};
   check(memcmp(&input.D3DDeclaration._M_start[output++],&expected,8)==0);offset+=sizes[type];
  }
  check(input.D3DDeclaration.size()==(supported?total:0));
  unsigned grown=scenario.used+(scenario.used>total-scenario.used?scenario.used:total-scenario.used);
  check(input.D3DDeclaration.capacity()==(total>scenario.capacity?grown:scenario.capacity));
  if(supported){
   D3DVERTEXELEMENT9 end={255,0,17,0,0,0};check(memcmp(&input.D3DDeclaration._M_start[output++],&end,8)==0);
  }
  for(unsigned index=output;index<total;++index){
   unsigned char expected[8];memset(expected,index<scenario.used?0xcd:0,8);
   check(memcmp(&input.D3DDeclaration._M_start[index],expected,8)==0);
  }
  free(input.D3DDeclaration._M_start);++count;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}
 printf("DECLARATION_GENERATION_CHAIN_BEHAVIOR_PASS scenarios=%u\n",count);return 0;
}
