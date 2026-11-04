#include "fable_game_text_bank.h"
#include "fable_disk_file.h"

// Retail 009CBA40.
CCharString CBankFile::GetBankHandle() const
{
    return BankHandle;
}

// Retail 009CBF10.
CWideString CBankFile::GetBankPathname() const
{
    return BankFile->GetPathname();
}

// Retail 009CBF90: reopen the bank file for writing, keeping the position.
void CBankFile::MakeWritable()
{
    if (!WrittenToFile) {
        long position = BankFile->GetPosition();
        BankFile->Close();
        BankFile->Open(Filename, FILE_ACCESS_READ_WRITE, 4);
        BankFile->SetPosition(position);
        WrittenToFile = true;
    }
}
