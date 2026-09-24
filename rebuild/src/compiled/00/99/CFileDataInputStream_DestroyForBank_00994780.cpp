#include "fable_ui_bank_stream.h"
void __fastcall FableUiDestroyBankStream(CFileDataInputStream* stream,void*)
{
    stream->__vftable=&FableUiFileInputStreamVtable;
    FableUiCloseBankStream(stream,0);
    stream->__vftable=FableUiDataInputStreamVtable;
    FableUiDestroyStreamBase(stream,0);
}
