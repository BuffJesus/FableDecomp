#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
void CBankFile::ReadEntryData(unsigned long index,unsigned long offset,void* data,unsigned long size)
{
    // Retail retains the packed-size query even though its result is unused.
    if (PackedDataSize.Size) (void)PackedDataSize[index-1];
    unsigned long position;
    if (PackedDataOffset.Size) position=PackedDataOffset[index-1];
    else position=RuntimeData[index].DataOffset;
    BankFile->SetPosition(position+offset);
    BankFile->Read(data,size,0);
}
