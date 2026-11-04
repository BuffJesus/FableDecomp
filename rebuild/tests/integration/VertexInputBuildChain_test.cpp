#include <stdio.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
static unsigned failed,bases;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass(){++bases;}
static void check(bool value){if(!value)++failed;}
struct ExpectedStream {unsigned index,count;CVertexStreamFormat::CDataElement elements[8];};
int main(){unsigned cases=0;const unsigned sizes[14]={4,8,12,16,4,4,8,4,2,4,6,8,4,4},types[8]={0,1,2,3,4,6,7,6};
 for(unsigned streamCount=1;streamCount<6;++streamCount)for(unsigned length=0;length<8;++length)for(unsigned seed=0;seed<14;++seed)for(unsigned alias=0;alias<2;++alias)for(unsigned eager=0;eager<2;++eager){
  unsigned before=bases;
  {
   CVertexShaderInput input;ExpectedStream expected[5];
   for(unsigned stream=0;stream<streamCount;++stream){
    expected[stream].index=57+stream;
    if(alias && stream){
     expected[stream].count=expected[0].count;memcpy(expected[stream].elements,expected[0].elements,expected[0].count*8);
     input.AddStream(expected[stream].index,input.DataStreams[0].StreamFormat,eager || stream+1==streamCount);
    }else{
     CVertexStreamFormat format;unsigned count=(length+stream)%8,totalSize=0;expected[stream].count=count;
     for(unsigned i=0;i<count;++i){unsigned type=(seed+i)%14;format.AddDataElement(0xfe+i,static_cast<EVertexStreamDataType>(type));expected[stream].elements[i].Register=0xfe+i;expected[stream].elements[i].DataType=static_cast<EVertexStreamDataType>(type);totalSize+=sizes[type];}
     check(format.CalculateVertexSize()==totalSize);input.AddStream(expected[stream].index,format,eager || stream+1==streamCount);
     if(count){check(input.DataStreams[stream].StreamFormat.DataElements._M_start!=format.DataElements._M_start);format.DataElements[0].Register=0xdeadbeef;}
    }
    check(input.DataStreams.size()==stream+1);
   }
   unsigned total=0,output=0,offset=0;bool supported=true;
   for(stream=0;stream<streamCount;++stream){
    check(input.DataStreams[stream].StreamIndex==expected[stream].index && input.DataStreams[stream].StreamFormat.DataElements.size()==expected[stream].count);
    if(expected[stream].count)check(memcmp(input.DataStreams[stream].StreamFormat.DataElements._M_start,expected[stream].elements,expected[stream].count*8)==0);
    total+=expected[stream].count+1;
    for(unsigned element=0;element<expected[stream].count;++element)if(expected[stream].elements[element].DataType>=8)supported=false;
   }
   check(input.D3DDeclaration.size()==(supported?total:0));
   if(supported){
    for(stream=0;stream<streamCount;++stream)for(unsigned element=0;element<expected[stream].count;++element){
     unsigned type=expected[stream].elements[element].DataType;D3DVERTEXELEMENT9 declaration={static_cast<unsigned short>(stream),static_cast<unsigned short>(offset),static_cast<unsigned char>(types[type]),0,5,static_cast<unsigned char>(expected[stream].elements[element].Register)};
     check(memcmp(&input.D3DDeclaration[output++],&declaration,8)==0);offset+=sizes[type];
    }
    D3DVERTEXELEMENT9 end={255,0,17,0,0,0};check(memcmp(&input.D3DDeclaration[output],&end,8)==0);
   }
  }
  check(bases>=before+streamCount+1);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("VERTEX_INPUT_BUILD_CHAIN_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
