#include "fable_console.h"
#include "fable_disk_file.h"
#include "fable_carray.h"
#include "fable_comments.h"
#include "fable_string_parser.h"

// lib_console.cpp, retail 009EC890 (Ego_r locals file, dst, parser,
// parsed_item): load the script, strip // and /* */ comments in place, then
// run it as one ";"-separated text command. The parser and its item are
// constructed but never used.
void CConsole::RunScript(const CWideString& filename)
{
    CDiskFileWin32 file;
    if (!CAFile::PathExists(filename))
        return;
    file.Open(filename, FILE_ACCESS_READ, 2);
    CArray<char> dst(file.GetLength() + 1);
    file.Read(&dst[0], file.GetLength(), 0);
    dst[file.GetLength()] = 0;
    GFRemoveComments(&dst[0], "//", "\n");
    GFRemoveComments(&dst[0], "//", "\r");
    GFRemoveComments(&dst[0], "//", "\0");
    GFRemoveComments(&dst[0], "/*", "*/");
    CStringParser parser(&dst[0], 0, CCharString(""), 0);
    CParsedItem parsed_item;
    RunTextCommand(dst.GetAsCArray());
}
