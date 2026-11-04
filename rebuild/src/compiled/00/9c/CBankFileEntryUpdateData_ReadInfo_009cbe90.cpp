#include "fable_bank_entry_update.h"
#include <string.h>

void CBankFileEntryUpdateData::ReadInfo(unsigned long offset,void* data,unsigned long size) const
{
    __int64 end=(unsigned long)(offset+size);
    if(end>=0 && end<=0x7fffffff)
        memcpy(data,InfoBuffer+offset,size);
}
