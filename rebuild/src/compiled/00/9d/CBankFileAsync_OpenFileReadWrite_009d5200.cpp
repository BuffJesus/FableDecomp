#include "fable_game_text_bank.h"

void CBankFileAsync::OpenFileReadWrite(const CWideString& filename,
                                      unsigned long openFlags,
                                      unsigned long flags)
{
    CBankFile::OpenFileReadWrite(filename, openFlags, flags);
    DisableAsync = true;
}
