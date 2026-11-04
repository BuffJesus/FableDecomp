#include "fable_string.h"
#include "fable_persist_context.h"
#include "fable_data_input_stream.h"
#include <stdio.h>
#include <string.h>
static unsigned overflow;
void CDataInputStream::ReadWithSrcChunkOverflow(void* output,long size){if(size!=4)return;memcpy(output,CurrentSourcePtr,4);CurrentSourcePtr+=4;SourceChunkBytesRemaining=0;StreamPos+=4;++overflow;}
int main(){unsigned long values[]={0,1,255,256,511,0xffffffffUL};for(unsigned i=0;i<6;++i)for(unsigned slow=0;slow<2;++slow){unsigned long raw[7]={0};CMemoryDataInputStream* stream=(CMemoryDataInputStream*)raw;stream->StreamPos=8;stream->CurrentSourcePtr=(unsigned char*)&values[i];stream->SourceChunkBytesRemaining=slow?2:8;overflow=0;unsigned char result=77;CPersistTraits<unsigned char> traits;traits.TransferBinaryIn(*stream,result);if(result!=(unsigned char)values[i]||stream->StreamPos!=12||stream->CurrentSourcePtr!=(unsigned char*)&values[i]+4||stream->SourceChunkBytesRemaining!=(slow?0:4)||overflow!=slow)return 1;}puts("PERSIST_BYTE_BINARY_OK cases=12");return 0;}
