#include "fable_font_data_banks.h"
#include "fable_date_and_time.h"

CDateAndTime CFontDataBank::GetLastModifiedTime(const CCharString&)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}
