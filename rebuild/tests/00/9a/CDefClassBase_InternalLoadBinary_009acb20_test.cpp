#include "fable_def_class_base.h"
#include "fable_data_input_stream.h"
#include <stdio.h>
#include <string.h>
static int overflows;
void CDataInputStream::ReadWithSrcChunkOverflow(void* p,long n){++overflows;memcpy(p,CurrentSourcePtr,n);CurrentSourcePtr+=n;StreamPos+=n;SourceChunkBytesRemaining=0;}
int main(){double rawDef[8],rawStream[4];CDefClassBase* def=reinterpret_cast<CDefClassBase*>(rawDef);CDataInputStream* stream=reinterpret_cast<CDataInputStream*>(rawStream);
 for(int mode=0;mode<2;++mode)for(int bits=0;bits<8;++bits){
 memset(rawDef,0xa5,sizeof(rawDef));memset(rawStream,0,sizeof(rawStream));unsigned char bytes[]={ (unsigned char)(bits&1?3:2),(unsigned char)(bits&2?255:254),(unsigned char)(bits&4?5:4)};
 stream->CurrentSourcePtr=bytes;stream->SourceChunkBytesRemaining=mode?0:3;overflows=0;
 def->CDefClassBase::InternalLoadBinary(*stream);
 if(reinterpret_cast<unsigned char*>(rawDef)[0x24]!=(unsigned char)(0xa0|bits) || stream->StreamPos!=3 || stream->CurrentSourcePtr!=bytes+3 || overflows!=(mode?3:0))return 1;
 if(reinterpret_cast<unsigned char*>(rawDef)[0x23]!=0xa5 || reinterpret_cast<unsigned char*>(rawDef)[0x25]!=0xa5)return 2;
 }
 puts("DEF_BASE_LOAD_PASS");return 0;}