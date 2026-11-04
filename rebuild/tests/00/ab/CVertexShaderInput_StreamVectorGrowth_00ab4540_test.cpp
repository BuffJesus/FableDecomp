#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
namespace _STL { template<> void vector<CVertexShaderInput::CStreamInfo>::_M_insert_overflow(CVertexShaderInput::CStreamInfo*,const CVertexShaderInput::CStreamInfo&,const __false_type&,size_t,bool); }
#include "../src/compiled/00/a5/CVertexStreamFormat_ElementVectors_00a5e0a0.cpp"
typedef CVertexShaderInput::CStreamInfo Stream;
typedef CVertexStreamFormat::CDataElement Element;
typedef _STL::vector<Stream> Streams;
static unsigned failed,bases;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass(){++bases;}
static void check(bool value){if(!value)++failed;}
static void make(Stream& value,unsigned index,unsigned nested){
 value.StreamIndex=index;unsigned n=(index+nested)%3;Element* data=n?static_cast<Element*>(malloc(n*8)):0;
 value.StreamFormat.DataElements._M_start=data;value.StreamFormat.DataElements._M_finish=data+n;value.StreamFormat.DataElements._M_end_of_storage._M_data=data+n;
 for(unsigned j=0;j<n;++j){data[j].Register=index*10+j;data[j].DataType=static_cast<EVertexStreamDataType>(j);}
}
struct Expected {unsigned index,count;Element data[2];};
static Expected snapshot(const Stream& stream){Expected e;e.index=stream.StreamIndex;e.count=stream.StreamFormat.DataElements.size();if(e.count)memcpy(e.data,stream.StreamFormat.DataElements._M_start,e.count*8);return e;}
int main(){unsigned cases=0;
 for(unsigned used=0;used<5;++used)for(unsigned pos=0;pos<=used;++pos)for(unsigned count=0;count<5;++count)for(unsigned atend=0;atend<2;++atend)for(unsigned alias=0;alias<2;++alias)for(unsigned nested=0;nested<3;++nested){
  if(alias && !used)continue;
  unsigned words[3],valueWords[5]={0};Streams& streams=*reinterpret_cast<Streams*>(words);Stream* initial=static_cast<Stream*>(malloc((used+2)*20));
  streams._M_start=initial;streams._M_finish=initial+used;streams._M_end_of_storage._M_data=initial+used+2;
  for(unsigned i=0;i<used;++i)make(initial[i],100+i,nested);
  Stream& external=*reinterpret_cast<Stream*>(valueWords);make(external,200,nested);const Stream& value=alias?initial[used/2]:external;
  Expected expected[12];unsigned resultCount=0;for(i=0;i<pos;++i)expected[resultCount++]=snapshot(initial[i]);for(i=0;i<count;++i)expected[resultCount++]=snapshot(value);if(!atend)for(i=pos;i<used;++i)expected[resultCount++]=snapshot(initial[i]);
  bases=0;streams._M_insert_overflow(initial+pos,value,_STL::__false_type(),count,atend!=0);
  check(streams.size()==resultCount && streams.capacity()==used+(used>count?used:count) && bases==used);
  for(i=0;i<resultCount;++i){check(streams[i].StreamIndex==expected[i].index && streams[i].StreamFormat.DataElements.size()==expected[i].count);if(expected[i].count)check(memcmp(streams[i].StreamFormat.DataElements._M_start,expected[i].data,expected[i].count*8)==0);free(streams[i].StreamFormat.DataElements._M_start);}
  free(streams._M_start);free(external.StreamFormat.DataElements._M_start);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("VERTEX_STREAM_GROWTH_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}

