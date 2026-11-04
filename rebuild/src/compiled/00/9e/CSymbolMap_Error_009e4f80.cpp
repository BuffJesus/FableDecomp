#include <stdio.h>
#include <stdarg.h>
#include "fable_symbol_map.h"

// lib_symbol_map.cpp, retail 009E4F80 (Ego_r locals string[256], error_out,
// line_string[10]; built with /GS): a compiler-style error line for the
// symbol file being parsed. The message is assembled and then dropped, since
// its output is compiled out of this build.
void __cdecl CSymbolMap::Error(const CWideString& file, long line, const char* text, ...)
{
    CCharString error_out;
    char line_string[10];
    _snprintf(line_string, 9, "(%d)", line + 1);
    line_string[9] = 0;
    char string[256];
    va_list args;
    va_start(args, text);
    vsprintf(string, text, args);
    error_out = CCharString("") + file.ToCharString() + line_string + " : error C2065 : " + string;
}
