// CDiskFileWin32::Write @ 0x0099AA90
// (FableWin ?Write@CDiskFileWin32@@UAEXPBXJPAVCASuspendableProcess@@@Z, slot 4).
//
// Validates the source (BAD_PARAMETERS), ignores empty writes, requires a
// read/write handle (NOT_OPEN_FOR_WRITING), writes through Win32 WriteFile
// (on failure: hard-disk error callback, then CANNOT_WRITE), and advances the
// cached position, growing the cached length when the write passed the end.
// Synchronous: the suspendable process is unused.

#include "fable_disk_file.h"
#include "fable_file_exception.h"

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableHandle;

extern "C" __declspec(dllimport) FableBool FABLE_STDCALL WriteFile(
    FableHandle file,
    const void* buffer,
    FableDword bytesToWrite,
    FableDword* bytesWritten,
    void* overlapped);

void CDiskFileWin32::Write(
    const void* data,
    long size,
    CASuspendableProcess*)
{
    if (data == 0)
    {
        CCharString message("Null dst parameter");
        throw CBBBFileException(message, CBBBFileException::BAD_PARAMETERS);
    }

    if (size != 0)
    {
        if (AccessType != FILE_ACCESS_READ_WRITE)
        {
            CCharString message("File is not open for writing");
            throw CBBBFileException(message, CBBBFileException::NOT_OPEN_FOR_WRITING);
        }

        FableDword written;
        if (WriteFile(WinFileHandle, data, size, &written, 0) != 1)
        {
            DoHardDiskErrorCallback("");
            CCharString message("Error writing file");
            throw CBBBFileException(message, CBBBFileException::CANNOT_WRITE);
        }

        Position += size;
        if (Position > Length)
            Length = Position;
    }
}