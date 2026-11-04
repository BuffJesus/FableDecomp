#include "fable_memory_file.h"
#include "fable_file_exception.h"
#include "fable_disk_file.h"
void CMemoryFile::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    CWideString fullPath = CAFile::ConvertPathToFullPath(pathname);
    if (CAFile::PathExists(fullPath))
    {
        CDiskFileWin32 file(pathname, accessType, flags);
        Length = file.GetLength();
        DataBuffer.resize(Length);
        if (Length > 0)
            file.Read(DataBuffer.begin(), file.GetLength(), 0);
        AccessType = accessType;
    }
    else
    {
        if (accessType == FILE_ACCESS_READ)
        {
            CCharString message = CCharString("File does not exist: ") + fullPath.ToCharString();
            throw CBBBFileException(message, CBBBFileException::BAD_PATH);
        }
        AccessType = accessType;
    }
    Pathname = pathname;
    Position = 0;
    Length = DataBuffer.size();
    Opened = true;
}
