// CDiskFileWin32::Clone @ 0x0099AE00
// (FableWin ?Clone@CDiskFileWin32@@UBEPAVCAFile@@XZ, vtable slot 14).
//
// Copies the file: the inline copy constructor opens a second read-only
// handle on the same path (flags 2).

#include "fable_disk_file.h"

CAFile* CDiskFileWin32::Clone() const
{
    return new CDiskFileWin32(*this);
}