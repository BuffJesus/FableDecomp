#include "fable_threaded_file_object.h"
#include "fable_bank_async_entry.h"
void CBankFileAsyncEntry::SetupRead(unsigned long offset, CBankFileAsyncData& data,
    unsigned long size, unsigned long, unsigned long, EThreadedFilePriority priority)
{
    if (BankFile.DisableAsync) {
        data.SetupBuffer(true);
        BankFile.ReadData(BankFile.GetEntryDataOffset(Index)+offset, data.Buffer, size);
        data.Finished=true;
    } else {
        CThreadedFileObjectDesc& desc=data.LoadDesc;
        desc.PDst=data.Buffer;
        if(BankFile.NonCached) {
            desc.FilePos=BankFile.GetEntryDataOffset(Index)+offset;
            desc.Size=BankFile.Align(size);
        } else {
            desc.FilePos=BankFile.GetEntryDataOffset(Index)+offset;
            desc.Size=size;
        }
        desc.PCallback=&data;
        desc.Priority=priority;
        desc.QueueWhenFinished=false;
        data.ThreadedFileUID=data.BankFile.ThreadedFile->AddObject(desc);
    }
}
