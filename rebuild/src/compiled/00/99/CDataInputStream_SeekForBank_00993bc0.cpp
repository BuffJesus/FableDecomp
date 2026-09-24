#include "fable_ui_bank_stream.h"
void __fastcall FableUiSeekBankStream(CFileDataInputStream* stream,void*,unsigned position)
{
    CDataInputStream* base=FableUiStreamPrefix(stream);
    unsigned end=base->StreamPos+static_cast<unsigned>(base->SourceChunkBytesRemaining);
    if(base->CurrentSourcePtr && position>=base->SourceChunkStreamPos && position<=end)
    {
        base->SourceChunkBytesRemaining=static_cast<long>(end-position);
        unsigned pointer=reinterpret_cast<unsigned>(base->CurrentSourcePtr)-base->StreamPos+position;
        base->CurrentSourcePtr=reinterpret_cast<void*>(pointer);
        base->StreamPos=position;
    }
    else
    {
        base->CurrentSourcePtr=0; base->SourceChunkStreamPos=0;
        base->SourceChunkBytesRemaining=0; base->StreamPos=position;
    }
}
