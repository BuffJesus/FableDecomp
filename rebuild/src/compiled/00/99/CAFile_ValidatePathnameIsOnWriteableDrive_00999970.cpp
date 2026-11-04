// lib_file_abstract.cpp, retail 0x009976D0, 0x00998570 and 0x00999970 (/GS):
// the narrow CAFile::SplitPath; GetRelativeFilePath, which expands it for path1
// and calls it for path2; and ValidatePathnameIsOnWriteableDrive, which expands it
// together with ConvertPathToFullPath (0x00999110, through
// FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE). With DebugUseReadOnlyDrives set,
// the release body only splits off the drive; the check on it is compiled out.
#define FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE
#include <stdlib.h>
#include <string.h>
#include "fable_filesystem.h"
#include "fable_string.h"
#include "fable_string_parser.h"

// Retail 009976D0.
void FABLE_FASTCALL CAFile::SplitPath(const CCharString& pathname, CCharString* pdrive, CCharString* pdirectory,
                                      CCharString* pfilename, CCharString* pextension)
{
    char drive[3];
    char dir[256];
    char fname[256];
    char ext[256];
    _splitpath(pathname, drive, dir, fname, ext);
    if (pdrive != 0)
        *pdrive = drive;
    if (pdirectory != 0)
        *pdirectory = dir;
    if (pfilename != 0)
        *pfilename = fname;
    if (pextension != 0)
        *pextension = ext;
}

// Retail 00999970 (Ego_r locals drive, fullpath).
void FABLE_FASTCALL CAFile::ValidatePathnameIsOnWriteableDrive(const CWideString& path)
{
    if (DebugUseReadOnlyDrives)
    {
        CCharString drive;
        CCharString fullpath = ConvertPathToFullPath(path).ToCharString();
        SplitPath(fullpath, &drive, 0, 0, 0);
    }
}

// Retail 00998570 (Ego_r locals): walk both directories with parsers. Drop
// the common leading directories, emit "..\" for each remaining directory of
// path1, then path2's remaining directories and its file name.
CCharString FABLE_FASTCALL CAFile::GetRelativeFilePath(const CCharString& path1_absolute,
                                                       const CCharString& path2_absolute)
{
    if (path1_absolute == path2_absolute)
        return CCharString("");
    CCharString path1_drive;
    CCharString path1_directory;
    SplitPath(path1_absolute, &path1_drive, &path1_directory, 0, 0);
    CCharString path2_drive;
    CCharString path2_directory;
    CCharString path2_filename;
    CCharString path2_extension;
    SplitPath(path2_absolute, &path2_drive, &path2_directory, &path2_filename, &path2_extension);
    CCharString path_relative;
    bool path_completed = false;
    CStringParser parser1(path1_directory, 0, CCharString(""), 0);
    CStringParser parser2(path2_directory, 0, CCharString(""), 0);
    do
    {
        CParsedItem parsed_item1;
        CParsedItem parsed_item2;
        bool directory_name_found_1 = parser1.ReadAsStringUntilString("\\", parsed_item1);
        bool directory_name_found_2 = parser2.ReadAsStringUntilString("\\", parsed_item2);
        if (!directory_name_found_1 && !directory_name_found_2)
        {
            path_relative = path2_filename + path2_extension;
            path_completed = true;
        }
        else
        {
            parser1.SkipPastString("\\");
            parser2.SkipPastString("\\");
            if (!directory_name_found_1 || !directory_name_found_2 ||
                parsed_item1.GetString() != parsed_item2.GetString())
            {
                if (directory_name_found_1)
                {
                    do
                    {
                        parser1.SkipPastNextItemIfIs("\\");
                        path_relative += "..\\";
                    } while (parser1.SkipUntilString("\\"));
                }
                if (directory_name_found_2)
                {
                    do
                    {
                        parser2.SkipPastNextItemIfIs("\\");
                        if (parsed_item2.GetString() != "")
                            path_relative += parsed_item2.GetString() + "\\";
                    } while (parser2.ReadAsStringUntilString("\\", parsed_item2));
                }
                path_relative += path2_filename + path2_extension;
                path_completed = true;
            }
        }
    } while (!path_completed);
    return path_relative;
}
