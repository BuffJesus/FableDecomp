#include "fable_game_text_bank.h"
#include "fable_date_and_time.h"

// Retail 009CB710 (manifest label CStreamingFontDataBank is a neighbour mix-up;
// that class's copy is 00A5F9D0).
CDateAndTime NGameText::CDataBank::GetLastModifiedTime(const CCharString&)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}
