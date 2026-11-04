#include "fable_data_input_stream.h"
#include "fable_definition_table.h"
#include "fable_disk_file.h"

// CDefStringTable::LoadTable(bool) @ 0x009D4C30.
// Callees by retail bytes: 0x999230 CAFile::PathExists, 0x99a2f0 CAFile
// ctor, 0x99aed0 CWideString(), 0x99a6a0 CDiskFileWin32::Open, 0x994700
// CFileDataInputStream ctor, 0x9d4ab0 ReadTable, 0x994780 its dtor, 0x99a920
// CDiskFileWin32::Close, 0x99b510 ~CWideString, 0x99a300 CAFile dtor.
void CDefStringTable::LoadTable(bool readOnly)
{
    if (CAFile::PathExists(TableFilename))
    {
        CDiskFileWin32 file;
        file.Open(TableFilename, FileAccessType(1), 4);
        CFileDataInputStream stream(&file, 0x4000);
        ReadTable(stream);
        Locked = true;
        ReadOnly = readOnly;
    }
}
