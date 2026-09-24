#include "fable_ui_bank_stream.h"
void __fastcall FableUiReadBankStreamDirect(CFileDataInputStream* stream,void*,void* target,long length)
{
    unsigned position=static_cast<FableUiBankStreamVtable*>(stream->__vftable)->GetPosition(stream,0);
    FableUiStreamFileMethods(stream->File)->SetPosition(stream->File,0,position);
    FableUiReadStreamFile(stream->File,target,length);
}
