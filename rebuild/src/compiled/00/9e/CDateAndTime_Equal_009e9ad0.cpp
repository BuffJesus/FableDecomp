#include "fable_date_and_time.h"
bool CDateAndTime::operator==(const CDateAndTime& other) const {
 return Year==other.Year && Month==other.Month && Day==other.Day && Hour==other.Hour && Minute==other.Minute && Second==other.Second && Millisecond==other.Millisecond;
}

// Retail 009E9B20: operator== above is auto-inlined here.
bool CDateAndTime::operator!=(const CDateAndTime& other) const {
 return !(*this == other);
}
