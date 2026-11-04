// Recovered from retail instructions; shared ABI. Validated by native owned-open gate.
#include "fable_string_parser.h"
bool CStringParser::ReadAsStringUntilString(const CCharString& text,CParsedItem& item)
{
 const char* start=String+StringPos;
 if(SkipUntilString(text)) {
  item.InitAsString(CCharString(start,static_cast<long>(String+StringPos-start)));
  return true;
 }
 return false;
}
