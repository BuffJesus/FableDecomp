#include "fable_game_text_bank.h"

void CBankFileAsync::OpenReadWrite(const CCharString& filename,
                                  unsigned long openFlags,
                                  unsigned long flags)
{
    CBankFile::OpenReadWrite(filename, openFlags, flags);
    DisableAsync = true;
}
