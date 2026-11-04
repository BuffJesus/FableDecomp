#include "fable_disk_file.h"

// CDiskFileWin32::CDiskFileWin32(CWideString const&, FileAccessType, ulong)
// @ 0x0099AD80.
CDiskFileWin32::CDiskFileWin32(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    Open(pathname, accessType, flags);
}
