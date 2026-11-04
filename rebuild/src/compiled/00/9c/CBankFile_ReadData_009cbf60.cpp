#include "fable_game_text_bank.h"
#include "fable_disk_file.h"

// Retail 009CBF60: virtual SetPosition (slot 5) then Read (slot 3) on the bank disk file.
void CBankFile::ReadData(unsigned long offset, void* data, unsigned long size) const
{
    BankFile->SetPosition(offset);
    BankFile->Read(data, size, 0);
}
