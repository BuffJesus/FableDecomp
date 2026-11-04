// lib_file_abstract.cpp, CAFile path-syntax check, retail 0x00998A30, with the
// three look-aheads it expands in place: ParserIsFinished 0x009975D0,
// ParserIsComponentNext 0x009981D0 and ParserIsDriveLabelNext 0x00998210.
// They share this TU because the expansion needs their bodies. The readers
// they call (0x00997DA0, 0x00997E90, 0x00998010, 0x00998170) stay out of line,
// as in retail.
#include "fable_filesystem.h"
#include "fable_string.h"
#include "fable_string_parser.h"

// Retail 009975D0.
bool FABLE_FASTCALL CAFile::ParserIsFinished(CStringParser& parser)
{
    return !parser.NextItemExists();
}

// Retail 009981D0.
bool FABLE_FASTCALL CAFile::ParserIsComponentNext(CStringParser& parser)
{
    unsigned long pos = parser.StringPos;
    CCharString component("");
    bool result = ParserReadPastComponent(parser, component);
    parser.SetStringPos(pos);
    return result;
}

// Retail 00998210.
bool FABLE_FASTCALL CAFile::ParserIsDriveLabelNext(CStringParser& parser)
{
    unsigned long pos = parser.StringPos;
    CParsedItem item;
    bool result = ParserReadPastDriveLabel(parser, item);
    parser.SetStringPos(pos);
    return result;
}

// Retail 00998A30 (Ego_r locals): walk the path as drive label, separators
// and components. CExpectedTextType says which may come next; a path longer
// than MAX_PATH fails.
bool FABLE_FASTCALL CAFile::IsFilePathSyntaxValid(const CWideString& pathname_wide, bool absolute_path)
{
    CCharString pathname = pathname_wide.ToCharString();
    if (pathname.GetLength() > 260)
        return false;
    CStringParser parser(pathname, 0, CCharString(""), 0);
    CParsedItem parsed_item;
    CExpectedTextType expected_text_type;
    if (absolute_path)
        expected_text_type.Set(false, false, true, false);
    else
        expected_text_type.Set(true, false, false, false);
    bool valid = true;
    bool finished = false;
    do
    {
        if (finished)
            break;
        if (ParserIsSeparatorNext(parser))
        {
            valid = expected_text_type.SeparatorOK;
            expected_text_type.Set(true, false, false, false);
            ParserReadPastSeparator(parser);
        }
        else if (ParserIsComponentNext(parser))
        {
            valid = expected_text_type.ComponentOK;
            expected_text_type.Set(false, true, false, true);
            CCharString pointless;
            ParserReadPastComponent(parser, pointless);
        }
        else if (ParserIsDriveLabelNext(parser))
        {
            valid = expected_text_type.DriveLabelOK;
            expected_text_type.Set(false, true, false, false);
            ParserReadPastDriveLabel(parser, parsed_item);
        }
        else if (ParserIsFinished(parser))
        {
            valid = expected_text_type.FinishedOK;
            finished = true;
        }
        else
            valid = false;
    } while (valid);
    return valid && finished;
}
