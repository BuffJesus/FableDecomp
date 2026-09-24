#include "fable_ui_bank_stream.h"
void __fastcall FableUiGetBankStreamSource(CFileDataInputStream* stream,void*,void** source,long* length)
{
    unsigned position=static_cast<FableUiBankStreamVtable*>(stream->__vftable)->GetPosition(stream,0);
    unsigned remaining=static_cast<unsigned>(FableUiStreamPrefix(stream)->StreamSize)-position;
    unsigned capacity=static_cast<unsigned>(stream->BufferSize);
    *length=remaining<capacity ? remaining : capacity;
    FableUiStreamFileMethods(stream->File)->SetPosition(stream->File,0,position);
    FableUiReadStreamFile(stream->File,stream->Buffer,*length);
    *source=stream->Buffer;
}
