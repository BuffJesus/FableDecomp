#include "fable_game_text_bank.h"

// Retail 009CC4C0: file bytes not taken by the header, info block or entry data.
// FileSize is read before the GetTotalDataSize call, as a named local reproduces.
unsigned long CBankFile::GetUnusedSpace() const
{
    unsigned long space = FileSize;
    return space - FileInformationSize - HeaderSize - GetTotalDataSize();
}
