#include "fable_string.h"
#include "fable_data_input_stream.h"
#include "fable_persist_context.h"
#include <stdio.h>
#include <string.h>
static int overflows,bad;static const void* expected;
void CDataInputStream::ReadWithSrcChunkOverflow(void* out,long size){
 ++overflows;if(this!=expected||size!=4)bad=1;memcpy(out,CurrentSourcePtr,size);CurrentSourcePtr+=size;StreamPos+=size;SourceChunkBytesRemaining=0;
}
int main(){
 const long values[]={0,1,-1,0x7fffffff,(long)0x80000000UL,0x12345678};
 for(int slow=0;slow<2;++slow)for(int i=0;i<6;++i){
  double raw[4];memset(raw,0,sizeof(raw));CMemoryDataInputStream& stream=*reinterpret_cast<CMemoryDataInputStream*>(raw);
  stream.StreamPos=7;stream.SourceChunkStreamPos=3;stream.CurrentSourcePtr=(unsigned char*)&values[i];stream.SourceChunkBytesRemaining=slow?2:4;expected=&stream;overflows=0;
  CPersistTraits<long> traits;traits.DefaultVal=0x33445566;long out[3]={111,99,222};traits.TransferBinaryIn(stream,out[1]);
  if(out[1]!=values[i]||out[0]!=111||out[2]!=222||traits.DefaultVal!=0x33445566||stream.StreamPos!=11||stream.SourceChunkStreamPos!=3||stream.CurrentSourcePtr!=(unsigned char*)&values[i]+4||stream.SourceChunkBytesRemaining!=0||overflows!=slow||bad)return 1;
 }
 puts("TRAITS_LONG_PASS");return 0;
}