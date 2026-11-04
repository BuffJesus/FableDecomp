#include "fable_string_parser.h"

void CStringParser::Init(const char* string, ErrorCallbackFunction errorCallback,
                         const CCharString& filename, long baseLineOffset)
{
    String = string;
    StringPos = 0;
    Line = baseLineOffset;
    BaseLineOffset = baseLineOffset;
    Filename = filename;
    ErrorCallback = errorCallback;
    InQuotes = false;
}
