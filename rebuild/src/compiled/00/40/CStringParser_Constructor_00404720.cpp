#include "fable_string_parser.h"

// CStringParser::CStringParser @ 0x00404720. TempCharString holds a
// reference to the text so the buffer Init parses stays alive.
CStringParser::CStringParser(
    const CCharString& string,
    ErrorCallbackFunction errorCallback,
    const CCharString& filename,
    long baseLineOffset)
{
    TempCharString = string;
    Init(
        string.GetConstCharPtr(),
        errorCallback,
        filename,
        baseLineOffset);
}
