// CAFile::ExpandPath(const CWideString&, const CWideString&) @ 0x00998C90
// (FableWin ?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z). Resolves `path`
// against the directory `context`: absolute ("X:\" or "\\server") paths are
// kept, a rooted "\path" takes the context's drive letter, "..\" goes through
// GetAbsoluteFilePath, a leading ".\" is dropped, anything else is appended
// to the context (which gets a trailing backslash).
#include "fable_filesystem.h"

CWideString FABLE_FASTCALL CAFile::ExpandPath(const CWideString& path, const CWideString& context)
{
    CWideString result;
    const wchar_t* text = path.GetAsCharArray();
    CWideString directory(context);
    if (directory.GetLength() > 0)
    {
        long last = directory.GetLength() - 1;
        if (directory.GetAsCharArray()[last] != L'\\')
            directory += L"\\";
    }

    if ((path.GetLength() >= 3 && text[1] == L':' && text[2] == L'\\') ||
        (path.GetLength() >= 2 && text[0] == L'\\' && text[1] == L'\\'))
    {
        result = path;
    }
    else if (path.GetLength() > 0 && text[0] == L'\\')
    {
        CWideString drive;
        drive += static_cast<const wchar_t*>(context)[0];
        result = drive + L":" + path;
    }
    else if (path.GetLength() > 2 && text[0] == L'.' && text[1] == L'.')
    {
        result = GetAbsoluteFilePath(directory, path);
    }
    else if (path.GetLength() > 1 && text[0] == L'.')
    {
        result = directory + path.GetSubString(2, path.GetLength());
    }
    else
    {
        result = directory + path;
    }
    return result;
}