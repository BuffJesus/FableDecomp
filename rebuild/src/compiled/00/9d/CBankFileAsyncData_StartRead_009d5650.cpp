// Retail 009D5650; both donor PDBs identify private void __thiscall StartRead().
#include "fable_threaded_file_object.h"

void CBankFileAsyncData::StartRead()
{
    ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);
}
