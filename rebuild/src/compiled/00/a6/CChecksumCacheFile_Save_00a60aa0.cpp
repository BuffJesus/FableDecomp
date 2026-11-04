#include "fable_checksum_cache_file.h"
#include "fable_file_data_output_stream.h"
#include "fable_disk_file.h"
inline CAFile::CAFile() {}

void CChecksumCacheFile::Save()
{
    if (NeedsSaving) {
        CDiskFileWin32 file;
        file.Open(Filename, FILE_ACCESS_CREATE, 2);
        CFileDataOutputStream output(&file);
        unsigned long start = output.GetPosition();
        output.WriteSLONG(0);
        output.WriteSLONG(0);
        long written = 0;
        typedef _STL::map<CWideString, CEntry>::iterator Iterator;
        for (Iterator it(Entries._M_t._M_header._M_data->_M_left); it != Entries.end(); ++it) {
            if (CAFile::PathExists(it->first)) {
                CWideString path(it->first);
                output.WriteNullTerminatedString(path.ToCharString());
                output.WriteDateAndTime(it->second.TimeStamp);
                output.WriteULONG(it->second.Checksum);
                ++written;
            }
        }
        output.SetPosition(start);
        output.WriteSLONG(4);
        output.WriteSLONG(written);
        file.Close();
        NeedsSaving = false;
    }
}
