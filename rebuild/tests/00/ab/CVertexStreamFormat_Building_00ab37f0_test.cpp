#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_stream_format.h"
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
static unsigned failed;
static void check(bool value){if(!value)++failed;}
static unsigned sizeOf(unsigned type){const unsigned sizes[14]={4,8,12,16,4,4,8,4,2,4,6,8,4,4};return type<14?sizes[type]:0;}
int main(){unsigned cases=0;
 for(unsigned t=0;t<256;++t){check(CVertexStreamFormat::GetDataTypeSize(static_cast<EVertexStreamDataType>(t))==sizeOf(t));++cases;}
 const unsigned invalid[5]={0xffffffff,0xfffffffe,0x7fffffff,0x80000000,0xffffffff};
 for(unsigned v=0;v<5;++v){check(CVertexStreamFormat::GetDataTypeSize(static_cast<EVertexStreamDataType>(invalid[v]))==0);++cases;}
 {unsigned long raw[4];memset(raw,0xab,sizeof(raw));CVertexStreamFormat* value=new(raw) CVertexStreamFormat;check(value->DataElements._M_start==0 && value->DataElements._M_finish==0 && value->DataElements._M_end_of_storage._M_data==0);check(raw[0]!=0 && raw[0]!=0xabababab);value->~CVertexStreamFormat();++cases;}
 for(unsigned count=0;count<10;++count)for(unsigned seed=0;seed<18;++seed){
  for(unsigned extra=0;extra<3;++extra)for(unsigned valueIndex=0;valueIndex<3;++valueIndex){
   unsigned cap=count+(extra==2?3:extra);unsigned raw[4]={0};CVertexStreamFormat& format=*reinterpret_cast<CVertexStreamFormat*>(raw);
   CVertexStreamFormat::CDataElement* initial=cap?static_cast<CVertexStreamFormat::CDataElement*>(malloc(cap*8)):0;
   format.DataElements._M_start=initial;format.DataElements._M_finish=initial+count;format.DataElements._M_end_of_storage._M_data=initial+cap;
   unsigned sum=0;for(unsigned i=0;i<count;++i){initial[i].Register=100+i;initial[i].DataType=static_cast<EVertexStreamDataType>((seed+i)%18);sum+=sizeOf(initial[i].DataType);}
   if(extra==0 && valueIndex==0){check(format.CalculateVertexSize()==sum);++cases;}
   unsigned index=valueIndex==0?0:valueIndex==1?255:0xffffffff;
   format.AddDataElement(index,static_cast<EVertexStreamDataType>(seed));
   check(format.DataElements.size()==count+1 && format.DataElements.capacity()==(count==cap?count+(count?count:1):cap));
   for(i=0;i<count;++i)check(format.DataElements[i].Register==100+i && format.DataElements[i].DataType==static_cast<EVertexStreamDataType>((seed+i)%18));
   check(format.DataElements[count].Register==index && format.DataElements[count].DataType==static_cast<EVertexStreamDataType>(seed));
   check(format.CalculateVertexSize()==sum+sizeOf(seed));free(format.DataElements._M_start);++cases;
  }
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("VERTEX_STREAM_BUILDING_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
