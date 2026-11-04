#include "fable_bank_async_entry.h"

void CBankFileAsyncEntry::ReadDataNonAsync(unsigned long offset,void* data,unsigned long size)
{
    BankFile.ReadEntryData(Index,offset,data,size);
}
