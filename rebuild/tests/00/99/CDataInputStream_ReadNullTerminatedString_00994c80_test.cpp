#include "fable_data_input_stream.h"
#include "test_support/checksum_string_fixture.h"
#include "../src/compiled/00/99/CCharString_AppendChar_0099f180.cpp"
#include "../src/compiled/00/9a/CBasicString_char_AppendRepeat_009a0460.cpp"
static const unsigned char source[]={0xff,'a',0x80,'z',0,'t',0};
static unsigned overflowCalls;
void CDataInputStream::ReadWithSrcChunkOverflow(void* out,long size){require(size==1 && StreamPos<sizeof(source));memcpy(out,source+StreamPos,size);StreamPos+=size;++overflowCalls;}
int main(){
 for(unsigned start=0;start<3;++start)for(unsigned available=0;available<6;++available){
  unsigned char raw[sizeof(CDataInputStream)+8];memset(raw,0xA5,sizeof(raw));CDataInputStream* stream=reinterpret_cast<CDataInputStream*>(raw+4);
  unsigned offset=start==0?0:start==1?4:5;unsigned count=start==0?4:start==1?0:1;
  stream->StreamPos=offset;stream->StreamSize=sizeof(source);stream->CurrentSourcePtr=const_cast<unsigned char*>(source+offset);
  stream->SourceChunkStreamPos=offset;stream->SourceChunkBytesRemaining=available<sizeof(source)-offset?available:sizeof(source)-offset;unsigned initial=stream->SourceChunkBytesRemaining;overflowCalls=0;
  {CCharString value=stream->CDataInputStream::ReadNullTerminatedString();CCharStringData* data=dataOf(value);
   require((data?data->Data.StringLength:0)==count && stream->StreamPos==offset+count+1 && overflowCalls==(count+1>initial?count+1-initial:0));
   if(count)require(!memcmp(data->text,source+offset,count) && data->text[count]==0);
  }
  require(!liveData && raw[3]==0xA5 && raw[sizeof(raw)-4]==0xA5);
 }
 puts("READ_NULL_STRING_00994c80_PASS");return 0;
}
