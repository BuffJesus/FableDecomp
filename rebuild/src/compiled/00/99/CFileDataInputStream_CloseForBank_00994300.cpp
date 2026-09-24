#include "fable_ui_bank_stream.h"
void __fastcall FableUiCloseBankStream(CFileDataInputStream* stream,void*)
{
    if(stream->Buffer)
    {
        FableUiFreeStringBuffer(stream->Buffer);
        stream->Buffer=0; stream->BufferSize=0;
    }
    void* file=stream->File;
    if(file && FableUiStreamFileMethods(file)->CanSeek(file,0))
    {
        file=stream->File;
        FableUiStreamFileVtable* fileTable=FableUiStreamFileMethods(file);
        FableUiBankStreamVtable* streamTable=static_cast<FableUiBankStreamVtable*>(stream->__vftable);
        unsigned position=streamTable->GetPosition(stream,0);
        fileTable->SetPosition(stream->File,0,position);
    }
    stream->File=0;
    CDataInputStream* base=FableUiStreamPrefix(stream);
    base->StreamPos=0; base->StreamSize=0; base->SourceChunkStreamPos=0;
    base->SourceChunkBytesRemaining=0; base->CurrentSourcePtr=0;
}
