#include "fable_ui_bank_open.h"
CThreadedFile* __fastcall FableUiConstructThreadedFile(CThreadedFile* file,void*)
{
    FableUiConstructBase(file,0);
    file->vtable_=reinterpret_cast<unsigned>(FableUiThreadedFileVtable);
    new (&file->filenameStorage_) CWideString;
    file->openedForWrite_=false; file->openFlag_=false;
    file->fileHandle_=reinterpret_cast<void*>(-1);
    return file;
}
