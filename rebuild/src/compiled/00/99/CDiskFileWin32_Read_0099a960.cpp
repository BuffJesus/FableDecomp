// CDiskFileWin32::Read @ 0x0099A960
// (FableWin ?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z, slot 3).
//
// Validates the destination (BAD_PARAMETERS), ignores empty reads, refuses a
// read at the cached end (END_OF_FILE), reads through Win32 ReadFile
// (CANNOT_READ on failure) and advances the cached position by the byte count
// ReadFile reports. Synchronous: the suspendable process is unused.

#include "fable_disk_file.h"
#include "fable_file_exception.h"

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableHandle;

extern "C" __declspec(dllimport) FableBool FABLE_STDCALL ReadFile(
    FableHandle file,
    void* buffer,
    FableDword bytesToRead,
    FableDword* bytesRead,
    void* overlapped);

void CDiskFileWin32::Read(
    void* data,
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
        if (Position == Length)
        {
            CCharString message("Cannot read past end of file");
            throw CBBBFileException(message, CBBBFileException::END_OF_FILE);
        }

        FableDword read;
        if (ReadFile(WinFileHandle, data, size, &read, 0) != 1)
        {
            CCharString message("Cannot read file");
            throw CBBBFileException(message, CBBBFileException::CANNOT_READ);
        }

        Position += read;
    }
}
