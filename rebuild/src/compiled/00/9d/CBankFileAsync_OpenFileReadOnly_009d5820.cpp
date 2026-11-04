#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"

// Retail 009D5820: open the bank, then a threaded reader on the same path.
bool CBankFileAsync::OpenFileReadOnly(const CWideString& filename, unsigned long flags)
{
    if (!CBankFile::OpenFileReadOnly(filename, flags))
        return false;
    ThreadedFile.Reset(new CThreadedFile);
    ThreadedFile->Open(GetBankPathname(), NonCached);
    DisableAsync = false;
    return true;
}
