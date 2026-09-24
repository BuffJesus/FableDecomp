#include "fable_ui_bank_stream.h"
CFileDataInputStream* __fastcall FableUiConstructBankStream(CFileDataInputStream* stream,void*,void* file,unsigned size)
{
    FableUiConstructBase(stream,0);
    CDataInputStream* base=FableUiStreamPrefix(stream);
    base->StreamPos=0; base->StreamSize=0; base->CurrentSourcePtr=0;
    base->SourceChunkStreamPos=0; base->SourceChunkBytesRemaining=0;
    stream->__vftable=&FableUiFileInputStreamVtable;
    stream->File=static_cast<CAFile*>(file);
    if(size)
    {
        stream->Buffer=FableUiAllocateStringBuffer(size);
        stream->BufferSize=static_cast<long>(size);
        unsigned length=FableUiStreamFileMethods(file)->GetLength(file,0);
        base->CurrentSourcePtr=stream->Buffer;
        base->StreamPos=0; base->SourceChunkStreamPos=0;
        base->StreamSize=static_cast<long>(length); base->SourceChunkBytesRemaining=0;
        unsigned position=FableUiStreamFileMethods(file)->GetPosition(file,0);
        FableUiSeekBankStream(stream,0,position);
    }
    return stream;
}
