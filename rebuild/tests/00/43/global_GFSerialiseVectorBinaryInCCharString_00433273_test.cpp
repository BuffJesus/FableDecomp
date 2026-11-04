#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"
#include "fable_data_input_stream.h"
#include "fable_persist_context.h"
static int live, reads, resizes, overflows, bad;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
CCharString::CCharString() : storage_(0) { ++live; }
CCharString::CCharString(const char* text, long length) : storage_(0) {
 ++live; if(length < 0)length=(long)strlen(text);
 storage_=(CCharStringData*)malloc(sizeof(CCharStringData));
 storage_->Data.PData=(char*)malloc(length+1);memcpy(storage_->Data.PData,text,length);storage_->Data.PData[length]=0;
 storage_->Data.StringLength=length;storage_->owners=1;
}
CCharString::CCharString(const CCharString& x) : storage_(x.storage_) {++live;if(storage_)++storage_->owners;}
void CCharString::UnassignString(){if(storage_ && --storage_->owners==0){free(storage_->Data.PData);free(storage_);}storage_=0;}
CCharString::~CCharString(){UnassignString();--live;}
CCharString& CCharString::operator=(const CCharString& x){if(this==&x)return *this;if(x.storage_)++x.storage_->owners;UnassignString();storage_=x.storage_;return *this;}
template<> void _STL::vector<CCharString>::resize(size_t count,const CCharString& fill){
 ++resizes;if(!fill.IsEmpty() || count>4){bad=1;return;}
 while(size()>count){--_M_finish;_M_finish->~CCharString();}
 while(size()<count){new(_M_finish)CCharString(fill);++_M_finish;}
}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CDataInputStream::CDataInputStream(){StreamPos=0;StreamSize=0;CurrentSourcePtr=0;SourceChunkStreamPos=0;SourceChunkBytesRemaining=0;}
CMemoryDataInputStream::~CMemoryDataInputStream(){}
void CDataInputStream::SetPosition(unsigned long p){CurrentSourcePtr+=p-StreamPos;SourceChunkBytesRemaining-=p-StreamPos;StreamPos=p;}
#define FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
#include "test_support/data_input_stream_stubs.h"
CCharString CMemoryDataInputStream::ReadNullTerminatedString(){
 ++reads;const char* p=(const char*)CurrentSourcePtr;unsigned long n=(unsigned long)strlen(p)+1;
 if(StreamPos+n>(unsigned long)StreamSize)abort();SetPosition(StreamPos+n);return CCharString(p);
}
void CDataInputStream::ReadWithSrcChunkOverflow(void* p,long n){++overflows;if(n!=4)bad=2;memcpy(p,CurrentSourcePtr,n);CurrentSourcePtr+=n;StreamPos+=n;SourceChunkBytesRemaining=StreamSize-StreamPos;}
int main(){
 for(int slow=0;slow<2;++slow){
  const char* expected[]={"intro.bik","","credits.bik"};
  unsigned char data[80];long count=3;memcpy(data,&count,4);unsigned long len=4;
  for(int i=0;i<3;++i){unsigned long n=(unsigned long)strlen(expected[i])+1;memcpy(data+len,expected[i],n);len+=n;}
  CMemoryDataInputStream stream;stream.StreamSize=len;stream.StreamSource=data;stream.CurrentSourcePtr=data;stream.SourceChunkBytesRemaining=slow?0:len;
  unsigned long space[4];_STL::vector<CCharString> value;
  value._M_start=(CCharString*)space;value._M_finish=value._M_start;value._M_end_of_storage._M_data=value._M_start+4;
  new(value._M_finish++)CCharString("old");reads=resizes=overflows=0;
  GFSerialiseVectorBinaryIn(stream,value);
  if(value.size()!=3 || reads!=3 || resizes!=1 || overflows!=slow || stream.StreamPos!=len || bad)return 1;
  for(i=0;i<3;++i)if(strcmp(value[i].GetConstCharPtr(),expected[i]))return 2;
  count=0;memcpy(data,&count,4);stream.StreamPos=0;stream.CurrentSourcePtr=data;stream.SourceChunkBytesRemaining=4;
  GFSerialiseVectorBinaryIn(stream,value);
  if(!value.empty() || reads!=3 || resizes!=2 || stream.StreamPos!=4 || live)return 3;
  value._M_start=value._M_finish=value._M_end_of_storage._M_data=0;
 }
 puts("STRING_VECTOR_IN_PASS");return 0;
}
