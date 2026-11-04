// lib_file_abstract.cpp, retail 0x00997470-0x00997A60 and 0x009994A0-0x00999C4E
// (/GS): NFile::ConvertDriveLetterToNumber, CRT path splitting into stack
// buffers (Ego_r locals drive[3], dir[256], fname[256], ext[256]), MakePath,
// and the directory creators that expand SplitPath, ConvertDriveLetterToNumber
// and ConvertPathToFullPath (FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE). They share
// this TU because the expansion needs the bodies. The narrow SplitPath
// (0x009976D0) is in CAFile_ValidatePathnameIsOnWriteableDrive_00999970.cpp.
#define FABLE_CONVERT_PATH_TO_FULL_PATH_INLINE
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "fable_filesystem.h"
#include "fable_string.h"

// Retail 00997470: A or a is drive 1; anything else is 0.
long FABLE_FASTCALL NFile::ConvertDriveLetterToNumber(wchar_t letter)
{
    if (letter >= L'a' && letter <= L'z')
        return letter - (L'a' - 1);
    if (letter >= L'A' && letter <= L'Z')
        return letter - (L'A' - 1);
    return 0;
}

// Retail 00997620.
void FABLE_FASTCALL CAFile::SplitPath(const CWideString& pathname, CWideString* pdrive, CWideString* pdirectory,
                                      CWideString* pfilename, CWideString* pextension)
{
    wchar_t drive[3];
    wchar_t dir[256];
    wchar_t fname[256];
    wchar_t ext[256];
    _wsplitpath(pathname, drive, dir, fname, ext);
    if (pdrive != 0)
        *pdrive = drive;
    if (pdirectory != 0)
        *pdirectory = dir;
    if (pfilename != 0)
        *pfilename = fname;
    if (pextension != 0)
        *pextension = ext;
}

// Retail 00997780.
CWideString FABLE_FASTCALL CAFile::MakePath(const CWideString& drive, const CWideString& directory,
                                            const CWideString& filename, const CWideString& extension)
{
    wchar_t path_buffer[260];
    _wmakepath(path_buffer, drive, directory, filename, extension);
    return CWideString(path_buffer);
}

// Retail 00997800: the path with its extension replaced.
CWideString FABLE_FASTCALL CAFile::SetFilenameExtension(const CWideString& pathname, const CWideString& ext)
{
    CWideString drive;
    CWideString dir;
    CWideString fname;
    SplitPath(pathname, &drive, &dir, &fname, 0);
    return MakePath(drive, dir, fname, ext);
}

// Retail 00997930: the file name and extension (SplitPath expanded).
CWideString FABLE_FASTCALL CAFile::GetFilenameFromPathname(const CWideString& pathname)
{
    CWideString fname;
    CWideString ext;
    SplitPath(pathname, 0, 0, &fname, &ext);
    return fname + ext;
}

// Retail 00997880 (near match, not gated: retail re-tests `start != -1` after
// setting it and sizes the copy with shl): the path item ending at separator
// number `index` (0-based).
// A leading separator, or one after a drive colon, gives "\". Retail's
// backward walk also stops at index 0 and copies from 1.
bool FABLE_FASTCALL NFile::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item)
{
    unsigned long separators = 0;
    item[0] = 0;
    for (long i = 0; path[i] != 0; ++i)
    {
        if (path[i] == L'\\' && ++separators > index)
        {
            if (i == 0 || path[i - 1] == L':')
            {
                wcscat(item, L"\\");
                return true;
            }
            long start;
            long j = i - 1;
            for (;;)
            {
                if (j == -1)
                {
                    start = 0;
                    break;
                }
                wchar_t c = path[j];
                if (c == L'\\' || c == L':' || j == 0)
                {
                    start = j + 1;
                    break;
                }
                --j;
            }
            long length = i - start;
            memcpy(item, path + start, length * sizeof(wchar_t));
            item[length] = 0;
            return true;
        }
    }
    return false;
}

// Retail 009994A0 (Ego_r locals directory[261], fullpath, drive_name).
void FABLE_FASTCALL CAFile::CreateDirectoryW(const CWideString& pathname)
{
    wchar_t directory[261];
    CWideString fullpath = ExpandPath(pathname, GetCurrentPath());
    CWideString drive_name;
    SplitPath(fullpath, &drive_name, 0, 0, 0);
    wchar_t drive_letter = drive_name.GetAsCharArray()[0];
    if (drive_letter != 0)
    {
        unsigned short drive_number = (unsigned short)NFile::ConvertDriveLetterToNumber(drive_letter);
        int old_drive = _getdrive();
        _chdrive(drive_number);
        if (drive_number != 0)
        {
            unsigned long item = 0;
            if (NFile::GetItemFromPath(fullpath, item, directory) == true)
            {
                do
                {
                    if (_wchdir(directory) != 0)
                    {
                        if (_wmkdir(directory) == -1)
                            break;
                        if (_wchdir(directory) != 0)
                            break;
                    }
                    ++item;
                } while (NFile::GetItemFromPath(fullpath, item, directory) == true);
            }
        }
        if (old_drive != -1)
            _chdrive(old_drive);
    }
}

// Retail 00999620 (Ego_r locals fullpath, drive, directory, directory_path).
void FABLE_FASTCALL CAFile::CreateDirectoryForFile(const CWideString& filepath)
{
    CWideString fullpath = ConvertPathToFullPath(filepath);
    if (!PathExists(fullpath))
    {
        CWideString drive;
        CWideString directory;
        SplitPath(fullpath, &drive, &directory, 0, 0);
        CWideString directory_path = MakePath(drive, directory, L"", L"");
        if (!PathExists(directory_path))
            CreateDirectoryW(directory_path);
    }
}

union FableULargeInteger
{
    struct
    {
        unsigned long LowPart;
        unsigned long HighPart;
    };
    unsigned __int64 QuadPart;
};
extern "C" __declspec(dllimport) int __stdcall GetDiskFreeSpaceExW(const wchar_t*, FableULargeInteger*, FableULargeInteger*,
                                                                    FableULargeInteger*);

// Retail 00999AB0 (near match, not gated: same 415 bytes and instructions,
// but retail gives drive/directory/the expanded temp different stack slots).
// Ego_r locals: num_free_bytes, drive, directory, num_total_bytes,
// directory_path.
bool FABLE_FASTCALL CAFile::IsRoomForFile(long file_size, const CWideString& dst_path)
{
    FableULargeInteger num_free_bytes;
    CWideString drive;
    CWideString directory;
    FableULargeInteger num_total_bytes;
    SplitPath(ConvertPathToFullPath(dst_path), &drive, &directory, 0, 0);
    CWideString directory_path = MakePath(drive, directory, L"", L"");
    if (PathExists(directory_path) || (CreateDirectoryW(directory_path), PathExists(directory_path)))
    {
        if (GetDiskFreeSpaceExW(directory_path, &num_free_bytes, &num_total_bytes, 0) &&
            num_free_bytes.QuadPart >= (unsigned __int64)file_size)
            return true;
    }
    return false;
}
