// lib_file_abstract.cpp, retail 0x00998250 (Ego_r
// ?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z and its locals):
// resolve path2 against the directory of path1. Each "\"-separated item of
// path2 is appended, and ".." drops the last component. The final item after
// the last "\" is appended at the end.
#define FABLE_CHAR_STRING_EQUALS_FORCEINLINE
#include "fable_filesystem.h"
#include "fable_string.h"
#include "fable_string_parser.h"

CCharString FABLE_FASTCALL CAFile::GetAbsoluteFilePath(const CCharString& path1_absolute,
                                                       const CCharString& path2_relative_to_path1)
{
    if (path2_relative_to_path1 == "")
        return path1_absolute;
    CCharString path_absolute(path1_absolute);
    for (long i = path_absolute.GetLength() - 1; i >= 0; --i)
    {
        if (path_absolute.GetConstCharPtr()[i] == '\\')
        {
            path_absolute.Truncate(i);
            break;
        }
    }
    CStringParser parser2(path2_relative_to_path1, 0, CCharString(""), 0);
    CParsedItem parsed_item2;
    while (parser2.ReadAsStringUntilString("\\", parsed_item2))
    {
        if (parsed_item2.GetString() == "..")
        {
            for (long i = path_absolute.GetLength() - 1; i >= 0; --i)
            {
                if (path_absolute.GetConstCharPtr()[i] == '\\')
                {
                    path_absolute.Truncate(i);
                    break;
                }
            }
        }
        else
            path_absolute += "\\" + parsed_item2.GetString();
        parser2.SkipPastString("\\");
    }
    CCharString filename(path2_relative_to_path1);
    for (long i = path2_relative_to_path1.GetLength() - 1; i >= 0; --i)
    {
        if (path2_relative_to_path1.GetConstCharPtr()[i] == '\\')
        {
            filename = path2_relative_to_path1.GetSubString(i + 1, path2_relative_to_path1.GetLength());
            break;
        }
    }
    path_absolute += "\\" + filename;
    return path_absolute;
}
