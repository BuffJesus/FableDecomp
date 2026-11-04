// lib_file_abstract.cpp, retail 0x00998010 (Ego_r
// ?ParserReadPastComponent@CAFile@@CI_NAAVCStringParser@@AAVCCharString@@@Z):
// read one path component, a byte at a time, up to the next separator or the
// end of the parsed items. It succeeds when the component has no forbidden
// symbol and does not end in a space. A name ending in '.' only passes as "."
// or "..". Near match, not gated: retail keeps `component` in its stack slot
// and loads its storage once for both compares. Its own TU: the separator
// test (0x00997DA0) is an out-of-line call.
#define FABLE_CHAR_STRING_EQUALS_FORCEINLINE
#include "fable_filesystem.h"
#include "fable_string.h"
#include "fable_string_parser.h"

bool FABLE_FASTCALL CAFile::ParserReadPastComponent(CStringParser& parser, CCharString& component)
{
    if (ParserIsSeparatorNext(parser) || !parser.NextItemExists())
        return false;
    bool result = false;
    CCharString text(parser.String);
    unsigned long start = parser.StringPos;
    long pos = start;
    char last = ' ';
    while (pos < text.GetLength())
    {
        last = text.GetConstCharPtr()[pos];
        parser.SetStringPos(pos + 1);
        if (ParserIsSeparatorNext(parser) || !parser.NextItemExists())
            break;
        ++pos;
    }
    component = text.GetSubString(start, pos + 1);
    if (!IsForbiddenSymbolInComponent(component) && last != ' ' &&
        (last != '.' || component == "." || component == ".."))
        result = true;
    return result;
}
