#include "fable_font_data_banks.h"
#include "fable_date_and_time.h"

CDateAndTime CStreamingFontDataBank::GetLastModifiedTime(const CCharString&)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}
