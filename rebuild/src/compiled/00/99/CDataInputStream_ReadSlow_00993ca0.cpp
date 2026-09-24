#include "fable_ui_bank_stream.h"
#include <string.h>
void __fastcall FableUiReadBankStreamSlow(CFileDataInputStream* stream,void*,void* target,long length)
{
    CDataInputStream* prefix=FableUiStreamPrefix(stream);
    unsigned char* destination=static_cast<unsigned char*>(target);
    long available=prefix->SourceChunkBytesRemaining;
    if(available>0)
    {
        if(prefix->StreamPos+static_cast<unsigned>(available)<=0x7fffffffu)
        {
            memcpy(destination,prefix->CurrentSourcePtr,available);
            prefix->SourceChunkBytesRemaining-=available;
            prefix->CurrentSourcePtr=static_cast<unsigned char*>(prefix->CurrentSourcePtr)+available;
            prefix->StreamPos+=available;
        }
        destination+=available; length-=available;
    }
    FableUiBankStreamVtable* methods=static_cast<FableUiBankStreamVtable*>(stream->__vftable);
    if(methods->UseBuffer(stream,0,length))
    {
        while(length>0)
        {
            prefix->SourceChunkStreamPos=prefix->StreamPos;
            static_cast<FableUiBankStreamVtable*>(stream->__vftable)->GetSource(stream,0,&prefix->CurrentSourcePtr,&prefix->SourceChunkBytesRemaining);
            long count=length<prefix->SourceChunkBytesRemaining ? length : prefix->SourceChunkBytesRemaining;
            if(count>0 && prefix->StreamPos+static_cast<unsigned>(count)<=0x7fffffffu)
            {
                memcpy(destination,prefix->CurrentSourcePtr,count);
                prefix->CurrentSourcePtr=static_cast<unsigned char*>(prefix->CurrentSourcePtr)+count;
                prefix->SourceChunkBytesRemaining-=count;
                prefix->StreamPos+=count;
            }
            destination+=count; length-=count;
        }
    }
    else
    {
        unsigned position=prefix->StreamPos;
        static_cast<FableUiBankStreamVtable*>(stream->__vftable)->ReadDirect(stream,0,destination,length);
        prefix->StreamPos=position+length;
        prefix->SourceChunkBytesRemaining=0; prefix->CurrentSourcePtr=0; prefix->SourceChunkStreamPos=0;
    }
}
