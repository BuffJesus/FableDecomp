#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
typedef CVertexStreamFormat::CDataElement Element;
typedef CVertexShaderInput::CStreamInfo Stream;
static unsigned failed,copies,frees,bases,grows,generates,used,capacity,length,index;
static Element values[8],temporary[8],saved[8];
static unsigned long alternateWords[40];
static CVertexShaderInput* owner;
static void check(bool value){if(!value)++failed;}
extern "C" void __cdecl free(void* pointer){if(!owner)return;check(pointer==temporary);++frees;}
CBaseClass::~CBaseClass(){check(frees==(length!=0));++bases;}
namespace _STL {
template<> vector<Element>::vector(const vector<Element>& other) : _Vector_base<Element,allocator<Element> >(allocator<Element>()) {
 check(other.size()==length);if(length)check(memcmp(other._M_start,values,length*8)==0);
 _M_start=length?(copies?saved:temporary):0;_M_finish=_M_start+length;_M_end_of_storage._M_data=_M_finish;
 if(length)memcpy(_M_start,values,length*8);++copies;
}
template<> void vector<Stream>::_M_insert_overflow(Stream* position,const Stream& value,const __false_type&,size_t count,bool atEnd){
 check(this==&owner->DataStreams && used==::capacity && position==_M_finish && count==1 && atEnd);
 check(value.StreamIndex==index && value.StreamFormat.DataElements._M_start==(length?temporary:0));++grows;
 Stream* alternate=reinterpret_cast<Stream*>(alternateWords);if(used)memcpy(alternate,_M_start,used*20);
 alternate[used].StreamIndex=index;unsigned long* format=reinterpret_cast<unsigned long*>(&alternate[used].StreamFormat);*format=*reinterpret_cast<const unsigned long*>(&value.StreamFormat);
 alternate[used].StreamFormat.DataElements._M_start=length?saved:0;alternate[used].StreamFormat.DataElements._M_finish=alternate[used].StreamFormat.DataElements._M_start+length;alternate[used].StreamFormat.DataElements._M_end_of_storage._M_data=alternate[used].StreamFormat.DataElements._M_finish;
 if(length)memcpy(saved,values,length*8);_M_start=alternate;_M_finish=alternate+used+1;_M_end_of_storage._M_data=alternate+used+(used?used:1);
}
}
void CVertexShaderInput::GenerateD3DDeclaration(){check(this==owner && bases==1 && frees==(length!=0));++generates;}
int main(){unsigned cases=0;const unsigned indices[3]={0,255,0xffffffff},lengths[4]={0,1,4,7};
 for(used=0;used<4;++used)for(capacity=used;capacity<5;++capacity)for(unsigned l=0;l<4;++l)for(unsigned ix=0;ix<3;++ix)for(unsigned generate=0;generate<2;++generate)for(unsigned alias=0;alias<2;++alias){
  if(alias && !used)continue;length=lengths[l];index=indices[ix];copies=frees=bases=grows=generates=0;
  unsigned long raw[7]={0},streamWords[25]={0},formatWords[4]={0};owner=reinterpret_cast<CVertexShaderInput*>(raw);Stream* streams=reinterpret_cast<Stream*>(streamWords);
  owner->DataStreams._M_start=capacity?streams:0;owner->DataStreams._M_finish=owner->DataStreams._M_start+used;owner->DataStreams._M_end_of_storage._M_data=owner->DataStreams._M_start+capacity;
  for(unsigned i=0;i<used;++i){streams[i].StreamIndex=57+i;streamWords[5*i+1]=0x129d578;}
  CVertexStreamFormat& format=alias?streams[0].StreamFormat:*reinterpret_cast<CVertexStreamFormat*>(formatWords);
  format.DataElements._M_start=length?values:0;format.DataElements._M_finish=format.DataElements._M_start+length;format.DataElements._M_end_of_storage._M_data=format.DataElements._M_finish;
  for(i=0;i<length;++i){values[i].Register=0xff+i;values[i].DataType=static_cast<EVertexStreamDataType>(i%8);}
  owner->AddStream(index,format,generate!=0);
  check(copies==(used==capacity?1:2) && grows==(used==capacity) && frees==(length!=0) && bases==1 && generates==generate);
  check(owner->DataStreams.size()==used+1 && owner->DataStreams[used].StreamIndex==index);
  check(owner->DataStreams[used].StreamFormat.DataElements._M_start==(length?saved:0));
  if(length)check(memcmp(saved,values,length*8)==0);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("VERTEX_INPUT_ADD_STREAM_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}


