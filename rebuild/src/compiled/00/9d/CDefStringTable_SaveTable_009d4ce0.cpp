#include "fable_def_string.h"
#include "fable_disk_file.h"
#include "fable_file_data_output_stream.h"

void CDefStringTable::SaveTable(bool lockAfterSave)
{
    if (!Locked)
    {
        CDiskFileWin32 file;
        file.Open(TableFilename, FILE_ACCESS_CREATE, 4);
        CFileDataOutputStream output(&file);
        WriteTable(output);
        if (lockAfterSave)
        {
            StringMap.clear();
            Locked = true;
        }
    }
}
